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

    def add_decision_record(self, dst, name, kind, record_id, author, previous="BRIEF-1", target="Yok"):
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
"""
        path = dst / "docs/decisions/active" / name
        path.write_text(body, encoding="utf-8")

    def add_complete_decision(self, dst, final_previous=None):
        self.add_decision_record(dst, "proposal-1.md", "Proposal", "P-1", "art-director")
        self.add_decision_record(dst, "proposal-2.md", "Proposal", "P-2", "technical-director")
        self.add_decision_record(dst, "proposal-3.md", "Proposal", "P-3", "finance-monetization-director")
        self.add_decision_record(dst, "critique.md", "Critique", "C-1", "game-director", target="P-1")
        self.add_decision_record(dst, "red-team.md", "Red-Team Report", "RT-1", "independent-red-team-auditor", target="P-1,P-2,P-3")
        previous = final_previous or "P-1,P-2,P-3,C-1,RT-1"
        self.add_decision_record(dst, "final.md", "Final Decision Record", "F-1", "studio-orchestrator", previous=previous)

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
        self.assertTrue(any("red-team report yazarı proposal sahibiyle aynı" in error for error in self.errors_after(mutate)))

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


if __name__ == "__main__":
    unittest.main()
