import json
from pathlib import Path
from shutil import copytree, rmtree
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from validate_studio import ROOT, REQUIRED_HEADINGS, validate, validate_repository


class StudioValidatorTests(unittest.TestCase):
    def fixture(self):
        tmp = tempfile.TemporaryDirectory()
        dst = Path(tmp.name) / "repo"
        copytree(ROOT, dst, ignore=lambda _path, _names: {".git", "__pycache__"})
        return tmp, dst

    def errors_after(self, mutate):
        tmp, dst = self.fixture()
        try:
            mutate(dst)
            return validate(dst)
        finally:
            tmp.cleanup()

    def add_decision_record(self, dst, name, kind, record_id, author,
                            previous="NOT APPLICABLE", target="NOT APPLICABLE", extra=""):
        body = f"""# {kind}

## Decision ID
DEC-TEST

## Record ID
{record_id}

## Author Agent ID
{author}

## Created Date
2026-08-08

## Source References
SRC-1

## Conflict of Interest Declaration
Yok

## Previous Stage References
{previous}

## Next Stage
Next

## Review Target
{target}

## Independence Declaration
Bağımsız

{extra}
"""
        path = dst / "docs/decisions/active" / name
        path.write_text(body, encoding="utf-8")

    def add_complete_decision(self, dst, final_previous=None):
        self.add_decision_record(dst, "brief.md", "Decision Brief", "B-1", "studio-orchestrator")
        self.add_decision_record(dst, "research.md", "Research Memo", "R-1", "market-research-director", previous="B-1", target="B-1")
        owners = ("art-director", "technical-director", "finance-monetization-director")
        for number, owner in enumerate(owners, 1):
            self.add_decision_record(dst, f"proposal-{number}.md", "Proposal", f"P-{number}", owner, previous="B-1,R-1", target="B-1,R-1")
            critique_name = "critique.md" if number == 1 else f"critique-{number}.md"
            self.add_decision_record(dst, critique_name, "Critique", f"C-{number}", "game-director", previous=f"P-{number}", target=f"P-{number}")
            self.add_decision_record(dst, f"rebuttal-{number}.md", "Rebuttal", f"RB-{number}", owner, previous=f"P-{number},C-{number}", target=f"C-{number}")
        self.add_decision_record(dst, "scorecard.md", "Scorecard", "S-1", "executive-producer", previous="RB-1,RB-2,RB-3", target="P-1,P-2,P-3", extra="## Stop-gates\nPASS")
        self.add_decision_record(dst, "red-team.md", "Red-Team Report", "RT-1", "independent-red-team-auditor", previous="S-1", target="P-1,P-2,P-3")
        all_prior = "B-1,R-1,P-1,P-2,P-3,C-1,C-2,C-3,RB-1,RB-2,RB-3,S-1,RT-1"
        self.add_decision_record(dst, "final.md", "Final Decision Record", "F-1", "studio-orchestrator", previous=final_previous or all_prior, target="RT-1", extra="## Durum\nDRAFT")

    def test_repository_is_valid_and_emits_semantic_warnings(self):
        errors, warnings = validate_repository(ROOT)
        self.assertEqual([], errors)
        self.assertTrue(any("re-audit" in warning for warning in warnings))

    def test_manifest_inventory_positive(self):
        manifest = json.loads((ROOT / "studio.manifest.json").read_text(encoding="utf-8"))
        self.assertTrue(all((ROOT / rel).is_file() for rel in manifest["required_files"]))
        self.assertTrue(all((ROOT / rel).is_dir() for rel in manifest["required_directories"]))

    def test_missing_manifest_file(self):
        errors = self.errors_after(lambda dst: (dst / "studio.manifest.json").unlink())
        self.assertTrue(any("Eksik manifest dosyası" in error for error in errors))

    def test_missing_manifest_required_file(self):
        errors = self.errors_after(lambda dst: (dst / "docs/production/roadmap.md").unlink())
        self.assertTrue(any("Eksik zorunlu dosya" in error for error in errors))

    def test_missing_required_directory(self):
        def mutate(dst):
            rmtree(dst / "docs/decisions/inbox")
        self.assertTrue(any("Eksik zorunlu dizin" in error for error in self.errors_after(mutate)))

    def test_missing_agent_heading(self):
        def mutate(dst):
            path = dst / "docs/agents/art-director.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Uzmanlık alanları", "## Alan", 1), encoding="utf-8")
        self.assertTrue(any("eksik başlık: Uzmanlık alanları" in error for error in self.errors_after(mutate)))

    def test_all_agent_headings_accept_utf8_turkish(self):
        text = (ROOT / "docs/agents/accessibility-ethics-reviewer.md").read_text(encoding="utf-8")
        self.assertIn("Erişilebilirlik", text)
        self.assertTrue(all(f"## {heading}" in text for heading in REQUIRED_HEADINGS))

    def test_duplicate_agent_id(self):
        def mutate(dst):
            source = dst / "docs/agents/studio-orchestrator.md"
            (dst / "docs/agents/duplicate.md").write_text(source.read_text(encoding="utf-8"), encoding="utf-8")
        self.assertTrue(any("Tekrarlanan Agent ID" in error for error in self.errors_after(mutate)))

    def test_missing_required_agent(self):
        errors = self.errors_after(lambda dst: (dst / "docs/agents/art-director.md").unlink())
        self.assertTrue(any("Eksik zorunlu agent rolü: art-director" in error for error in errors))

    def test_unknown_reporting_role(self):
        def mutate(dst):
            path = dst / "docs/agents/art-director.md"
            path.write_text(path.read_text(encoding="utf-8").replace("`creative-director`", "`missing-role`", 1), encoding="utf-8")
        self.assertTrue(any("mevcut olmayan raporlama rolü" in error for error in self.errors_after(mutate)))

    def test_unknown_supervised_role(self):
        def mutate(dst):
            path = dst / "docs/agents/art-director.md"
            text = path.read_text(encoding="utf-8").replace("`concept-art-lead`", "`missing-role`", 1)
            path.write_text(text, encoding="utf-8")
        self.assertTrue(any("bilinmeyen denetlenen rol" in error for error in self.errors_after(mutate)))

    def test_independent_role_cannot_report_to_orchestrator(self):
        def mutate(dst):
            path = dst / "docs/agents/legal-ip-risk-advisor.md"
            path.write_text(path.read_text(encoding="utf-8").replace("`FOUNDER`", "`studio-orchestrator`", 1), encoding="utf-8")
        self.assertTrue(any("Bağımsız rol doğrudan FOUNDER" in error for error in self.errors_after(mutate)))

    def test_orchestrator_cannot_supervise_independent_role(self):
        def mutate(dst):
            path = dst / "docs/agents/studio-orchestrator.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Denetlediği roller\nYok", "## Denetlediği roller\n`legal-ip-risk-advisor`", 1), encoding="utf-8")
        self.assertTrue(any("Yasak bağımsızlık ilişkisi" in error for error in self.errors_after(mutate)))

    def test_orchestrator_cannot_claim_all_specialists(self):
        def mutate(dst):
            path = dst / "docs/agents/studio-orchestrator.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Denetlediği roller\nYok", "## Denetlediği roller\nTüm uzman roller", 1), encoding="utf-8")
        errors = self.errors_after(mutate)
        self.assertTrue(any("tüm uzman rolleri" in error for error in errors))

    def test_founder_authority_delegation_is_rejected(self):
        def mutate(dst):
            path = dst / "docs/studio/governance.md"
            path.write_text(path.read_text(encoding="utf-8") + "\nStudio Orchestrator Stüdyo adı kararını verebilir.\n", encoding="utf-8")
        self.assertTrue(any("Kurucu yetkisi başka role devrediliyor" in error for error in self.errors_after(mutate)))

    def test_missing_founder_right(self):
        def mutate(dst):
            path = dst / "docs/studio/founder-rights.md"
            path.write_text(path.read_text(encoding="utf-8").replace("- Stüdyo adı\n", ""), encoding="utf-8")
        self.assertTrue(any("Kurucu korunmuş yetkisi eksik" in error for error in self.errors_after(mutate)))

    def test_missing_codeowners(self):
        errors = self.errors_after(lambda dst: (dst / ".github/CODEOWNERS").unlink())
        self.assertTrue(any(".github/CODEOWNERS" in error for error in errors))

    def test_missing_github_actions_workflow(self):
        errors = self.errors_after(lambda dst: (dst / ".github/workflows/studio-validation.yml").unlink())
        self.assertTrue(any("studio-validation.yml" in error for error in errors))

    def test_empty_critical_document(self):
        def mutate(dst):
            (dst / "docs/studio/governance.md").write_text("", encoding="utf-8")
        self.assertTrue(any("Kritik belge boş/yetersiz" in error for error in self.errors_after(mutate)))

    def test_placeholder_critical_document(self):
        def mutate(dst):
            path = dst / "docs/studio/governance.md"
            path.write_text(path.read_text(encoding="utf-8") + "\nREPLACE_WITH_POLICY\n", encoding="utf-8")
        self.assertTrue(any("placeholder içeriyor" in error for error in self.errors_after(mutate)))

    def test_missing_agents_ip_link(self):
        def mutate(dst):
            path = dst / "AGENTS.md"
            text = path.read_text(encoding="utf-8").replace("- [Fikrî mülkiyet politikası](docs/studio/intellectual-property-policy.md)\n", "")
            path.write_text(text, encoding="utf-8")
        self.assertTrue(any("intellectual-property-policy.md" in error for error in self.errors_after(mutate)))

    def test_missing_agents_quality_gate_link(self):
        def mutate(dst):
            path = dst / "AGENTS.md"
            text = path.read_text(encoding="utf-8").replace("- [Kalite kapıları](docs/studio/quality-gates.md)\n", "")
            path.write_text(text, encoding="utf-8")
        self.assertTrue(any("quality-gates.md" in error for error in self.errors_after(mutate)))

    def test_missing_decision_template(self):
        errors = self.errors_after(lambda dst: (dst / "docs/decisions/templates/proposal.md").unlink())
        self.assertTrue(any("proposal.md" in error for error in errors))

    def test_missing_decision_template_field(self):
        def mutate(dst):
            path = dst / "docs/decisions/templates/proposal.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Review Target", "## Hedef", 1), encoding="utf-8")
        self.assertTrue(any("eksik karar alanı: Review Target" in error for error in self.errors_after(mutate)))

    def test_complete_decision_chain_is_valid(self):
        tmp, dst = self.fixture()
        try:
            self.add_complete_decision(dst)
            self.assertEqual([], validate(dst))
        finally:
            tmp.cleanup()

    def test_final_requires_three_distinct_proposal_owners(self):
        def mutate(dst):
            self.add_complete_decision(dst)
            path = dst / "docs/decisions/active/proposal-3.md"
            path.write_text(path.read_text(encoding="utf-8").replace("finance-monetization-director", "technical-director"), encoding="utf-8")
        self.assertTrue(any("üç farklı proposal sahibi" in error for error in self.errors_after(mutate)))

    def test_critique_author_cannot_own_target_proposal(self):
        def mutate(dst):
            self.add_complete_decision(dst)
            path = dst / "docs/decisions/active/critique.md"
            path.write_text(path.read_text(encoding="utf-8").replace("game-director", "art-director"), encoding="utf-8")
        self.assertTrue(any("critique yazarı proposal sahibiyle aynı" in error for error in self.errors_after(mutate)))

    def test_red_team_author_cannot_own_target_proposal(self):
        def mutate(dst):
            self.add_complete_decision(dst)
            path = dst / "docs/decisions/active/red-team.md"
            path.write_text(path.read_text(encoding="utf-8").replace("independent-red-team-auditor", "technical-director"), encoding="utf-8")
        self.assertTrue(any("red-team author must be" in error for error in self.errors_after(mutate)))

    def test_final_references_every_prior_record(self):
        def mutate(dst):
            self.add_complete_decision(dst, final_previous="P-1,P-2,P-3,C-1")
        self.assertTrue(any("final kayıt önceki aşamalara referans vermiyor" in error for error in self.errors_after(mutate)))

    def test_broken_inline_and_reference_links(self):
        def mutate(dst):
            path = dst / "README.md"
            path.write_text(path.read_text(encoding="utf-8") + "\n[bad](missing.md)\n[ref]: absent.md\n", encoding="utf-8")
        errors = self.errors_after(mutate)
        self.assertGreaterEqual(sum("bozuk iç bağlantı" in error for error in errors), 2)

    def test_cli_nonzero_exit_code(self):
        tmp, dst = self.fixture()
        try:
            (dst / ".github/CODEOWNERS").unlink()
            result = subprocess.run([sys.executable, str(dst / "scripts/validate_studio.py")], capture_output=True, text=True, encoding="utf-8", errors="replace")
            self.assertNotEqual(0, result.returncode)
            self.assertIn("STUDIO VALIDATION FAILED", result.stdout)
        finally:
            tmp.cleanup()

    def test_reports_multiple_errors_at_once(self):
        def mutate(dst):
            (dst / ".github/CODEOWNERS").unlink()
            (dst / ".github/workflows/studio-validation.yml").unlink()
            (dst / "docs/decisions/templates/proposal.md").unlink()
        errors = self.errors_after(mutate)
        self.assertGreaterEqual(len(errors), 3)
        self.assertTrue(any("CODEOWNERS" in error for error in errors))
        self.assertTrue(any("studio-validation.yml" in error for error in errors))

    def decision_errors(self, mutate):
        def wrapped(dst):
            self.add_complete_decision(dst)
            mutate(dst)
        return self.errors_after(wrapped)

    def test_decision_brief_missing(self):
        self.assertTrue(any("decision brief" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/brief.md").unlink())))

    def test_research_memo_missing(self):
        self.assertTrue(any("research memo" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/research.md").unlink())))

    def test_only_two_proposals(self):
        def mutate(d):
            for name in ("proposal-3.md", "critique-3.md", "rebuttal-3.md"):
                (d / "docs/decisions/active" / name).unlink()
        self.assertTrue(any("proposal" in e for e in self.decision_errors(mutate)))

    def test_three_proposals_same_agent(self):
        def mutate(d):
            for name in ("proposal-2.md", "proposal-3.md"):
                path = d / "docs/decisions/active" / name
                text = path.read_text(encoding="utf-8")
                text = text.replace("technical-director", "art-director").replace("finance-monetization-director", "art-director")
                path.write_text(text, encoding="utf-8")
        self.assertTrue(any("üç farklı proposal sahibi" in e for e in self.decision_errors(mutate)))

    def test_one_proposal_critique_missing(self):
        self.assertTrue(any("proposal has no critique" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/critique-2.md").unlink())))

    def test_one_proposal_rebuttal_missing(self):
        self.assertTrue(any("proposal has no rebuttal" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/rebuttal-2.md").unlink())))

    def test_rebuttal_wrong_agent(self):
        def mutate(d):
            path = d / "docs/decisions/active/rebuttal-2.md"
            path.write_text(path.read_text(encoding="utf-8").replace("technical-director", "game-director"), encoding="utf-8")
        self.assertTrue(any("rebuttal author" in e for e in self.decision_errors(mutate)))

    def test_scorecard_missing(self):
        self.assertTrue(any("scorecard" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/scorecard.md").unlink())))

    def test_red_team_missing(self):
        self.assertTrue(any("red-team report" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/red-team.md").unlink())))

    def test_red_team_wrong_role(self):
        def mutate(d):
            path = d / "docs/decisions/active/red-team.md"
            path.write_text(path.read_text(encoding="utf-8").replace("independent-red-team-auditor", "game-director"), encoding="utf-8")
        self.assertTrue(any("red-team author must be" in e for e in self.decision_errors(mutate)))

    def test_red_team_does_not_target_all_proposals(self):
        def mutate(d):
            path = d / "docs/decisions/active/red-team.md"
            path.write_text(path.read_text(encoding="utf-8").replace("P-1,P-2,P-3", "P-1,P-2"), encoding="utf-8")
        self.assertTrue(any("red-team does not target every proposal" in e for e in self.decision_errors(mutate)))

    def test_final_decision_missing(self):
        self.assertTrue(any("final decision record" in e for e in self.decision_errors(lambda d: (d / "docs/decisions/active/final.md").unlink())))

    def test_empty_review_target(self):
        def mutate(d):
            path = d / "docs/decisions/active/critique.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Review Target\nP-1", "## Review Target\n"), encoding="utf-8")
        self.assertTrue(any("empty" in e or "missing or empty" in e for e in self.decision_errors(mutate)))

    def test_unknown_record_reference(self):
        def mutate(d):
            path = d / "docs/decisions/active/research.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Review Target\nB-1", "## Review Target\nUNKNOWN"), encoding="utf-8")
        self.assertTrue(any("unknown Record ID" in e for e in self.decision_errors(mutate)))

    def test_critique_targets_only_proposals(self):
        def mutate(d):
            path = d / "docs/decisions/active/critique.md"
            path.write_text(path.read_text(encoding="utf-8").replace("## Review Target\nP-1", "## Review Target\nP-1,R-1"), encoding="utf-8")
        self.assertTrue(any("critique must target only" in e for e in self.decision_errors(mutate)))

    def test_scorecard_must_evaluate_all_proposals(self):
        def mutate(d):
            path = d / "docs/decisions/active/scorecard.md"
            path.write_text(path.read_text(encoding="utf-8").replace("P-1,P-2,P-3", "P-1,P-2"), encoding="utf-8")
        self.assertTrue(any("scorecard does not evaluate every proposal" in e for e in self.decision_errors(mutate)))

    def test_rebuttal_must_reference_related_critique(self):
        def mutate(d):
            path = d / "docs/decisions/active/rebuttal-2.md"
            path.write_text(path.read_text(encoding="utf-8").replace("P-2,C-2", "P-2,C-1"), encoding="utf-8")
        self.assertTrue(any("related critique" in e for e in self.decision_errors(mutate)))

    def test_cross_decision_reference(self):
        def mutate(d):
            path = d / "docs/decisions/active/brief.md"
            path.write_text(path.read_text(encoding="utf-8").replace("DEC-TEST", "DEC-OTHER"), encoding="utf-8")
        self.assertTrue(any("cross-Decision ID" in e for e in self.decision_errors(mutate)))

    def test_duplicate_record_id(self):
        def mutate(d):
            path = d / "docs/decisions/active/proposal-2.md"
            path.write_text(path.read_text(encoding="utf-8").replace("P-2", "P-1", 1), encoding="utf-8")
        self.assertTrue(any("Duplicate Record ID" in e for e in self.decision_errors(mutate)))

    def test_stop_gate_fail_cannot_be_approved(self):
        def mutate(d):
            score = d / "docs/decisions/active/scorecard.md"
            score.write_text(score.read_text(encoding="utf-8").replace("PASS", "FAIL"), encoding="utf-8")
            final = d / "docs/decisions/active/final.md"
            final.write_text(final.read_text(encoding="utf-8").replace("DRAFT", "APPROVED"), encoding="utf-8")
        self.assertTrue(any("blocking stop-gate" in e for e in self.decision_errors(mutate)))

    def test_stop_gate_not_reviewed_cannot_be_approved(self):
        def mutate(d):
            score = d / "docs/decisions/active/scorecard.md"
            score.write_text(score.read_text(encoding="utf-8").replace("PASS", "NOT REVIEWED"), encoding="utf-8")
            final = d / "docs/decisions/active/final.md"
            final.write_text(final.read_text(encoding="utf-8").replace("DRAFT", "APPROVED"), encoding="utf-8")
        self.assertTrue(any("blocking stop-gate" in e for e in self.decision_errors(mutate)))

    def test_founder_topic_requires_founder_decision(self):
        def mutate(d):
            brief = d / "docs/decisions/active/brief.md"
            brief.write_text(brief.read_text(encoding="utf-8") + "\n## Konu\nStüdyo adı\n", encoding="utf-8")
        self.assertTrue(any("requires Founder Decision" in e for e in self.decision_errors(mutate)))

    def test_codeowners_wrong_founder_owner(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text(path.read_text(encoding="utf-8").replace("@adomm16", "@wrong"), encoding="utf-8")
        self.assertTrue(any("founder owner missing" in e for e in self.errors_after(mutate)))

    def test_codeowners_global_rule_missing(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text("\n".join(line for line in path.read_text(encoding="utf-8").splitlines() if not line.startswith("* ")), encoding="utf-8")
        self.assertTrue(any("global '*' rule" in e for e in self.errors_after(mutate)))

    def test_codeowners_critical_pattern_missing(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text(path.read_text(encoding="utf-8").replace("/docs/audits/ @adomm16\n", ""), encoding="utf-8")
        self.assertTrue(any("/docs/audits/" in e for e in self.errors_after(mutate)))

    def test_codeowners_owner_only_in_comment(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text("# @adomm16\n* @wrong\n", encoding="utf-8")
        self.assertTrue(any("founder owner missing" in e for e in self.errors_after(mutate)))

    def test_codeowners_empty_owner(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text(path.read_text(encoding="utf-8") + "\n/empty/\n", encoding="utf-8")
        self.assertTrue(any("owner missing" in e for e in self.errors_after(mutate)))

    def test_codeowners_invalid_pattern(self):
        def mutate(d):
            path = d / ".github/CODEOWNERS"
            path.write_text(path.read_text(encoding="utf-8") + "\ndocs\\bad @adomm16\n", encoding="utf-8")
        self.assertTrue(any("invalid or ineffective pattern" in e for e in self.errors_after(mutate)))

    def workflow_errors(self, replace_from, replace_to):
        def mutate(d):
            path = d / ".github/workflows/studio-validation.yml"
            path.write_text(path.read_text(encoding="utf-8").replace(replace_from, replace_to), encoding="utf-8")
        return self.errors_after(mutate)

    def test_workflow_pull_request_missing(self):
        self.assertTrue(any("pull_request" in e for e in self.workflow_errors("  pull_request:\n", "")))

    def test_workflow_wrong_name(self):
        self.assertTrue(any("Workflow name" in e for e in self.workflow_errors("name: studio-validation", "name: wrong")))

    def test_workflow_validator_command_missing(self):
        self.assertTrue(any("validate_studio.py" in e for e in self.workflow_errors("python scripts/validate_studio.py", "python -V")))

    def test_workflow_unit_command_missing(self):
        self.assertTrue(any("unittest discover" in e for e in self.workflow_errors("python -m unittest discover -s tests -v", "python -V")))

    def test_workflow_command_only_in_comment(self):
        self.assertTrue(any("validate_studio.py" in e for e in self.workflow_errors("run: python scripts/validate_studio.py", "# run: python scripts/validate_studio.py")))

    def test_workflow_masks_failure(self):
        self.assertTrue(any("masks failures" in e for e in self.workflow_errors("python scripts/validate_studio.py", "python scripts/validate_studio.py || true")))

    def test_workflow_continue_on_error(self):
        self.assertTrue(any("continue-on-error" in e for e in self.workflow_errors("runs-on: ubuntu-latest", "runs-on: ubuntu-latest\n    continue-on-error: true")))

    def test_workflow_checkout_missing(self):
        self.assertTrue(any("checkout" in e for e in self.workflow_errors("actions/checkout@v4", "example/no-checkout@v4")))

    def test_workflow_python_setup_missing(self):
        self.assertTrue(any("Python setup" in e for e in self.workflow_errors("actions/setup-python@v5", "example/no-python@v5")))

    def test_valid_codeowners_and_workflow_positive(self):
        self.assertEqual([], validate(ROOT))

    def test_agent_profile_identical_section_emits_warning(self):
        tmp, dst = self.fixture()
        try:
            for path in (dst / "docs/agents").glob("*.md"):
                if path.name in {"index.md", "shared-professional-policy.md"}:
                    continue
                text = path.read_text(encoding="utf-8")
                start = text.index("## Temel misyon")
                end = text.index("## Uzmanlık alanları")
                path.write_text(text[:start] + "## Temel misyon\nAynı operasyonel metin.\n\n" + text[end:], encoding="utf-8")
            errors, warnings = validate_repository(dst)
            self.assertEqual([], errors)
            self.assertTrue(any("Agent profile regression" in w for w in warnings))
        finally:
            tmp.cleanup()

    def test_founder_authority_scan_covers_readme(self):
        def mutate(d):
            path = d / "README.md"
            path.write_text(path.read_text(encoding="utf-8") + "\nStudio Orchestrator Stüdyo adı kararını verebilir.\n", encoding="utf-8")
        self.assertTrue(any("README.md" in e and "Kurucu yetkisi" in e for e in self.errors_after(mutate)))

    def test_founder_authority_audit_quote_is_excluded(self):
        def mutate(d):
            path = d / "docs/audits/governance/example-history.md"
            path.write_text("Geçmiş alıntı: Studio Orchestrator Stüdyo adı kararını verebilir.\n", encoding="utf-8")
        self.assertEqual([], self.errors_after(mutate))


if __name__ == "__main__":
    unittest.main()
