#!/usr/bin/env python3
"""Deterministic repository governance checks.

Semantic intent that cannot be proved reliably is reported as a warning, never
as a fabricated guarantee. Errors are reserved for structural evidence.
"""
from __future__ import annotations

import json
from pathlib import Path
import re
import sys
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = "studio.manifest.json"
PLACEHOLDER_RE = re.compile(
    r"(?i)(REPLACE_WITH_|\bTBD\b|\bTODO\b|buraya\s+(?:yaz|ekle))"
)


def _read(path: Path, errors: list[str], label: str) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        errors.append(f"UTF-8 okunamıyor: {label}")
    except OSError as exc:
        errors.append(f"Dosya okunamadı: {label}: {exc}")
    return ""


def _section(text: str, heading: str) -> str | None:
    match = re.search(
        rf"(?ms)^## {re.escape(heading)}[ \t]*\r?\n(.*?)(?=^## |\Z)", text
    )
    return match.group(1).strip() if match else None


def _scalar(text: str, heading: str) -> str | None:
    value = _section(text, heading)
    if value is None or not value.strip():
        return None
    return value.strip().strip("`").splitlines()[0].strip()


def _references(value: str | None, not_applicable: str = "NOT APPLICABLE") -> set[str]:
    if not value or value.strip().casefold() in {"yok", "none", not_applicable.casefold()}:
        return set()
    return {item.strip() for item in re.split(r"[,;\n]", value) if item.strip()}


def _load_manifest(root: Path, errors: list[str]) -> dict:
    path = root / MANIFEST
    if not path.is_file():
        errors.append(f"Eksik manifest dosyası: {MANIFEST}")
        return {}
    try:
        data = json.loads(_read(path, errors, MANIFEST))
    except json.JSONDecodeError as exc:
        errors.append(f"Geçersiz manifest JSON: {exc}")
        return {}
    error_count = len(errors)
    _validate_manifest_schema(data, errors)
    return {} if len(errors) > error_count else data


def _policy_list(data: dict, key: str, errors: list[str]) -> list:
    value = data.get(key)
    if not isinstance(value, list) or not value:
        errors.append(f"Manifest policy must be a non-empty list: {key}")
        return []
    fingerprints = [json.dumps(item, ensure_ascii=False, sort_keys=True) for item in value]
    if len(fingerprints) != len(set(fingerprints)):
        errors.append(f"Manifest policy contains duplicate entries: {key}")
    return value


def _validate_manifest_schema(data: dict, errors: list[str]) -> None:
    list_keys = (
        "required_files", "required_directories", "required_agents", "critical_files",
        "agent_profile_exclusions", "agent_profile_required_headings",
        "protected_founder_rights", "required_agents_links",
        "decision_metadata_required_fields", "independent_roles",
        "required_reporting_relationships", "forbidden_reporting_relationships",
        "forbidden_supervisor_relationships", "forbidden_supervision_claims",
    )
    policies = {key: _policy_list(data, key, errors) for key in list_keys}
    for key in ("decision_chain", "repository_protection", "agent_profile_regression"):
        if not isinstance(data.get(key), dict) or not data[key]:
            errors.append(f"Manifest policy must be a non-empty object: {key}")
    for key in ("required_files", "required_directories", "required_agents", "critical_files",
                "agent_profile_exclusions", "agent_profile_required_headings",
                "protected_founder_rights", "required_agents_links",
                "decision_metadata_required_fields", "independent_roles"):
        if any(not isinstance(item, str) or not item.strip() for item in policies[key]):
            errors.append(f"Manifest policy entries must be non-empty strings: {key}")

    agents = set(item for item in policies["required_agents"] if isinstance(item, str))
    independent = set(item for item in policies["independent_roles"] if isinstance(item, str))
    for role in sorted(independent - agents):
        errors.append(f"Manifest independent role is unknown: {role}")

    relation_specs = {
        "required_reporting_relationships": ("role", "reports_to"),
        "forbidden_reporting_relationships": ("role", "reports_to"),
        "forbidden_supervisor_relationships": ("supervisor", "role"),
        "forbidden_supervision_claims": ("supervisor", "text"),
    }
    normalized: dict[str, set[tuple[str, str]]] = {}
    for key, fields in relation_specs.items():
        normalized[key] = set()
        for item in policies[key]:
            if not isinstance(item, dict) or set(item) != set(fields) or any(
                    not isinstance(item.get(field), str) or not item[field] for field in fields):
                errors.append(f"Manifest relationship entry has invalid schema: {key}")
                continue
            relation = (item[fields[0]], item[fields[1]])
            normalized[key].add(relation)
            for field, role in zip(fields, relation):
                if field == "text":
                    continue
                if role != "FOUNDER" and role not in agents:
                    errors.append(f"Manifest relationship references unknown role: {key}: {role}")
    overlap = normalized["required_reporting_relationships"] & normalized["forbidden_reporting_relationships"]
    for role, reports_to in sorted(overlap):
        errors.append(f"Manifest relationship is both required and forbidden: {role} -> {reports_to}")

    chain = data.get("decision_chain", {})
    stages = chain.get("stages") if isinstance(chain, dict) else None
    if not isinstance(stages, dict) or not stages:
        errors.append("Manifest policy must define non-empty decision_chain.stages")
    else:
        for stage, limits in stages.items():
            if not isinstance(stage, str) or not stage or not isinstance(limits, dict):
                errors.append("Manifest decision stage has invalid schema")
                continue
            minimum, maximum = limits.get("minimum"), limits.get("maximum")
            if not isinstance(minimum, int) or minimum < 0:
                errors.append(f"Manifest decision stage minimum is invalid: {stage}")
            if maximum is not None and (not isinstance(maximum, int) or
                                        (isinstance(minimum, int) and maximum < minimum)):
                errors.append(f"Manifest decision stage maximum is invalid: {stage}")
        final_stage = chain.get("final_stage")
        if not isinstance(final_stage, str) or final_stage not in stages:
            errors.append("Manifest decision_chain.final_stage must reference a defined stage")

    protection = data.get("repository_protection", {})
    scalar_keys = ("founder_github_owner", "required_workflow_file", "required_workflow_name",
                   "required_job_id", "required_job_display_name", "required_status_check")
    list_policy_keys = ("critical_codeowners_patterns", "required_triggers",
                        "required_push_branches", "required_commands", "required_actions",
                        "forbidden_failure_masking_patterns", "supported_yaml_subset")
    if isinstance(protection, dict):
        for key in scalar_keys:
            if not isinstance(protection.get(key), str) or not protection[key].strip():
                errors.append(f"Manifest repository protection field missing/invalid: {key}")
        for key in list_policy_keys:
            value = protection.get(key)
            if not isinstance(value, list) or not value or any(not isinstance(item, str) or not item for item in value):
                errors.append(f"Manifest repository protection list missing/invalid: {key}")
            elif len(value) != len(set(value)):
                errors.append(f"Manifest repository protection list contains duplicates: {key}")
        for pattern in protection.get("forbidden_failure_masking_patterns", []):
            try:
                re.compile(pattern)
            except (re.error, TypeError):
                errors.append(f"Manifest failure masking regex is invalid: {pattern}")


def _validate_inventory(root: Path, manifest: dict, errors: list[str]) -> None:
    for rel in manifest.get("required_files", []):
        if not (root / rel).is_file():
            errors.append(f"Eksik zorunlu dosya: {rel}")
    for rel in manifest.get("required_directories", []):
        if not (root / rel).is_dir():
            errors.append(f"Eksik zorunlu dizin: {rel}")


def _validate_profiles(root: Path, manifest: dict, errors: list[str], warnings: list[str]) -> dict[str, dict]:
    profiles: dict[str, dict] = {}
    required_headings = manifest.get("agent_profile_required_headings", [])
    agents_dir = root / "docs/agents"
    exclusions = set(manifest.get("agent_profile_exclusions", []))
    for path in sorted(agents_dir.glob("*.md")) if agents_dir.is_dir() else []:
        if path.name in exclusions:
            continue
        rel = path.relative_to(root).as_posix()
        text = _read(path, errors, rel)
        actual_headings = re.findall(r"(?m)^## ([^\r\n]+?)\s*$", text)
        for heading in required_headings:
            if _section(text, heading) is None:
                errors.append(f"{rel}: eksik başlık: {heading}")
        for heading in sorted(set(actual_headings) - set(required_headings)):
            errors.append(f"{rel}: manifestte tanımsız agent başlığı: {heading}")
        aid = _scalar(text, "Agent ID")
        if not aid:
            errors.append(f"{rel}: Agent ID okunamadı")
            continue
        if aid in profiles:
            errors.append(f"Tekrarlanan Agent ID: {aid}")
        profiles[aid] = {"path": path, "text": text}

    required = set(manifest.get("required_agents", []))
    for aid in sorted(required - profiles.keys()):
        errors.append(f"Eksik zorunlu agent rolü: {aid}")
    expected_count = manifest.get("agent_count")
    if isinstance(expected_count, int) and len(profiles) != expected_count:
        errors.append(f"Agent sayısı uyuşmuyor: manifest={expected_count}, bulunan={len(profiles)}")

    for aid, profile in profiles.items():
        rel = profile["path"].relative_to(root).as_posix()
        reports = _scalar(profile["text"], "Rapor verdiği rol")
        if not reports:
            errors.append(f"{rel}: raporlama rolü okunamadı")
        elif reports != "FOUNDER" and reports not in profiles:
            errors.append(f"{rel}: mevcut olmayan raporlama rolü: {reports}")
        supervised = _section(profile["text"], "Denetlediği roller") or ""
        refs = re.findall(r"`([^`]+)`", supervised)
        if supervised.strip() != "Yok" and not refs:
            errors.append(f"{rel}: denetlenen roller Agent ID olarak yazılmalı")
        for role in refs:
            if role not in profiles:
                errors.append(f"{rel}: bilinmeyen denetlenen rol: {role}")
        profile["reports"] = reports
        profile["supervises"] = refs

    for relation in manifest.get("required_reporting_relationships", []):
        aid, reports_to = relation.get("role"), relation.get("reports_to")
        if aid in profiles and profiles[aid].get("reports") != reports_to:
            errors.append(f"Zorunlu raporlama ilişkisi sağlanmıyor: {aid} -> {reports_to}")
    for relation in manifest.get("forbidden_reporting_relationships", []):
        aid, reports_to = relation.get("role"), relation.get("reports_to")
        if aid in profiles and profiles[aid].get("reports") == reports_to:
            errors.append(f"Yasak raporlama ilişkisi: {aid} -> {reports_to}")
    for relation in manifest.get("forbidden_supervisor_relationships", []):
        supervisor, role = relation.get("supervisor"), relation.get("role")
        if supervisor in profiles and role in profiles[supervisor].get("supervises", []):
            errors.append(f"Yasak bağımsızlık ilişkisi: {supervisor} -> {role}")
    for claim in manifest.get("forbidden_supervision_claims", []):
        supervisor, text = claim.get("supervisor"), claim.get("text")
        if supervisor in profiles and text in (_section(profiles[supervisor]["text"], "Denetlediği roller") or ""):
            errors.append(f"Yasak denetim iddiası: {supervisor}: {text}")
    regression = manifest.get("agent_profile_regression", {})
    allowed = set(regression.get("allowed_shared_sections", []))
    if regression.get("warn_when_identical_across_all_profiles") and len(profiles) > 1:
        for heading in required_headings:
            if heading in allowed or heading in {"Agent ID", "Kurumsal unvan"}:
                continue
            values = [(_section(profile["text"], heading) or "").strip() for profile in profiles.values()]
            if values and values[0] and len(set(values)) == 1:
                warnings.append(
                    f"Agent profile regression: '{heading}' is identical across every profile; human review required"
                )
    return profiles


def _validate_markdown_links(root: Path, errors: list[str]) -> None:
    inline = re.compile(r"(?<!!)\[[^]]+\]\(\s*<?([^)>\s]+)>?(?:\s+[^)]*)?\)")
    ref_def = re.compile(r"(?m)^\s*\[[^]]+\]:\s*<?([^>\s]+)>?")
    for path in root.rglob("*.md"):
        rel = path.relative_to(root).as_posix()
        text = _read(path, errors, rel)
        for link in inline.findall(text) + ref_def.findall(text):
            link = unquote(link)
            if link.startswith(("http://", "https://", "mailto:", "#")):
                continue
            target_rel = link.split("#", 1)[0]
            if not target_rel:
                continue
            target = (path.parent / target_rel).resolve()
            if not target.exists():
                errors.append(f"{rel}: bozuk iç bağlantı: {link}")


def _validate_founder_authority(root: Path, manifest: dict, errors: list[str], warnings: list[str]) -> None:
    founder_path = root / "docs/studio/founder-rights.md"
    founder = _read(founder_path, errors, "docs/studio/founder-rights.md") if founder_path.exists() else ""
    protected = manifest.get("protected_founder_rights", [])
    documented = re.findall(r"(?m)^-\s+(.+?)\s*$", founder)
    for item in protected:
        if item not in founder:
            errors.append(f"Kurucu korunmuş yetkisi eksik: {item}")
    for item in sorted(set(documented) - set(protected)):
        errors.append(f"Kurucu yetkisi belgede var ancak manifest politikasında yok: {item}")
    delegation = re.compile(r"(?i)(agent|orchestrator|director|lead).{0,100}(onaylayabilir|kararını verebilir|yetkilidir|devredilir)")
    for path in sorted(root.rglob("*.md")):
        rel = path.relative_to(root).as_posix()
        if path == founder_path or rel.startswith("docs/audits/"):
            continue
        in_fence = False
        for line_no, line in enumerate(_read(path, errors, rel).splitlines(), 1):
            if line.lstrip().startswith("```"):
                in_fence = not in_fence
                continue
            stripped = line.strip()
            if in_fence or stripped.startswith(">"):
                continue
            explanatory = any(token in stripped.casefold() for token in (
                "yasak", "devredilemez", "yalnızca kurucu", "örnek", "açıklama",
            ))
            if not explanatory and any(item.casefold() in line.casefold() for item in protected) and delegation.search(line):
                errors.append(f"Kurucu yetkisi başka role devrediliyor: {rel}:{line_no}")
    warnings.append("Kurucu yetkisi semantik taraması sınırlıdır; bağımsız insan re-audit'i gerekir")


def _validate_critical(root: Path, manifest: dict, errors: list[str]) -> None:
    for rel in manifest.get("critical_files", []):
        path = root / rel
        if not path.is_file():
            continue
        text = _read(path, errors, rel).strip()
        if len(text) < 80:
            errors.append(f"Kritik belge boş/yetersiz: {rel}")
        elif (path.suffix.lower() in {".md", ".json", ".yml", ".yaml"} or path.name == "CODEOWNERS") and PLACEHOLDER_RE.search(text):
            errors.append(f"Kritik belge placeholder içeriyor: {rel}")


def _validate_agents_links(root: Path, manifest: dict, errors: list[str]) -> None:
    path = root / "AGENTS.md"
    text = _read(path, errors, "AGENTS.md") if path.exists() else ""
    for link in manifest.get("required_agents_links", []):
        if link not in text:
            errors.append(f"AGENTS.md zorunlu bağlantıyı içermiyor: {link}")


def _validate_decision_templates(root: Path, manifest: dict, errors: list[str]) -> None:
    templates = root / "docs/decisions/templates"
    for path in templates.glob("*.md") if templates.is_dir() else []:
        text = _read(path, errors, path.relative_to(root).as_posix())
        for field in manifest.get("decision_metadata_required_fields", []):
            if _section(text, field) is None:
                errors.append(f"{path.relative_to(root).as_posix()}: eksik karar alanı: {field}")

def _validate_complete_decisions(root: Path, manifest: dict, errors: list[str]) -> None:
    chain = manifest.get("decision_chain", {})
    stages = chain.get("stages", {})
    na = chain.get("not_applicable_value", "NOT APPLICABLE")
    decision_fields = manifest.get("decision_metadata_required_fields", [])
    records: list[dict] = []
    for state in ("active", "approved", "rejected"):
        base = root / "docs/decisions" / state
        for path in base.rglob("*.md") if base.is_dir() else []:
            text = _read(path, errors, path.relative_to(root).as_posix())
            record = {field: _scalar(text, field) for field in decision_fields}
            if not all(value and value.strip() for value in record.values()):
                errors.append(f"{path.relative_to(root).as_posix()}: decision record fields missing or empty")
                continue
            record.update(path=path, state=state, text=text,
                          kind=text.splitlines()[0].lstrip("# ").strip().casefold())
            records.append(record)
    index: dict[str, dict] = {}
    for record in records:
        rid = record["Record ID"]
        if rid in index:
            errors.append(f"Duplicate Record ID: {rid}")
        else:
            index[rid] = record
    groups: dict[str, list[dict]] = {}
    for record in records:
        groups.setdefault(record["Decision ID"], []).append(record)
    for decision_id, group in groups.items():
        by_kind = {kind: [r for r in group if r["kind"] == kind] for kind in stages}
        for kind, limits in stages.items():
            count = len(by_kind[kind])
            if count < limits.get("minimum", 0):
                errors.append(f"{decision_id}: missing required stage: {kind}")
            if count > limits.get("maximum", count):
                errors.append(f"{decision_id}: too many records for stage: {kind}")
        proposals = by_kind.get("proposal", [])
        proposal_ids = {r["Record ID"] for r in proposals}
        owner_by_proposal = {r["Record ID"]: r["Author Agent ID"] for r in proposals}
        if len(set(owner_by_proposal.values())) < chain.get("proposal_minimum_distinct_authors", 3):
            errors.append(f"{decision_id}: en az üç farklı proposal sahibi gerekli")
        for record in group:
            previous = _references(record["Previous Stage References"], na)
            targets = _references(record["Review Target"], na)
            for ref in previous | targets:
                target = index.get(ref)
                if target is None:
                    errors.append(f"{decision_id}: unknown Record ID reference: {ref}")
                elif target["Decision ID"] != decision_id:
                    errors.append(f"{decision_id}: cross-Decision ID reference: {ref}")
            if record["kind"] in {"critique", "rebuttal", "scorecard", "red-team report"} and not targets:
                errors.append(f"{decision_id}: empty Review Target for {record['kind']}")
        critiques = by_kind.get("critique", [])
        rebuttals = by_kind.get("rebuttal", [])
        critique_ids_all = {r["Record ID"] for r in critiques}
        for critique in critiques:
            targets = _references(critique["Review Target"], na)
            if not targets or not targets.issubset(proposal_ids):
                errors.append(f"{decision_id}: critique must target only valid proposal Record IDs")
        for rebuttal in rebuttals:
            targets = _references(rebuttal["Review Target"], na)
            if not targets or not targets.issubset(critique_ids_all):
                errors.append(f"{decision_id}: rebuttal Review Target must contain only critique Record IDs")
        for proposal in proposals:
            pid, owner = proposal["Record ID"], proposal["Author Agent ID"]
            matching_critiques = [r for r in critiques if pid in _references(r["Review Target"], na)]
            if not matching_critiques:
                errors.append(f"{decision_id}: proposal has no critique: {pid}")
            if any(r["Author Agent ID"] == owner for r in matching_critiques):
                errors.append(f"{decision_id}: critique yazarı proposal sahibiyle aynı: {pid}")
            matching_rebuttals = [r for r in rebuttals if pid in _references(r["Previous Stage References"], na)]
            if not matching_rebuttals:
                errors.append(f"{decision_id}: proposal has no rebuttal: {pid}")
            critique_ids = {r["Record ID"] for r in matching_critiques}
            for rebuttal in matching_rebuttals:
                refs = _references(rebuttal["Previous Stage References"], na)
                if rebuttal["Author Agent ID"] != owner:
                    errors.append(f"{decision_id}: rebuttal author must own proposal: {pid}")
                if not (refs & critique_ids):
                    errors.append(f"{decision_id}: rebuttal must reference proposal and related critique: {pid}")
        for scorecard in by_kind.get("scorecard", []):
            if not proposal_ids.issubset(_references(scorecard["Review Target"], na)):
                errors.append(f"{decision_id}: scorecard does not evaluate every proposal")
        for report in by_kind.get("red-team report", []):
            if report["Author Agent ID"] != chain.get("red_team_author"):
                errors.append(f"{decision_id}: red-team author must be {chain.get('red_team_author')}")
            if not proposal_ids.issubset(_references(report["Review Target"], na)):
                errors.append(f"{decision_id}: red-team does not target every proposal")
        final_stage = chain.get("final_stage")
        required_kinds = set(stages) - {final_stage}
        required_ids = {r["Record ID"] for r in group if r["kind"] in required_kinds}
        for final in by_kind.get(final_stage, []):
            missing = required_ids - _references(final["Previous Stage References"], na)
            if missing:
                errors.append(f"{decision_id}: final kayıt önceki aşamalara referans vermiyor: {', '.join(sorted(missing))}")
            chain_text = "\n".join(r["text"] for r in group)
            founder_topic = any(item.casefold() in chain_text.casefold()
                                for item in manifest.get("protected_founder_rights", []))
            founder_value = _section(final["text"], "Founder Decision") or _section(final["text"], "Kurucu kararı ve onay tarihi")
            if founder_topic and (not founder_value or founder_value.casefold() == na.casefold()):
                errors.append(f"{decision_id}: founder-protected topic requires Founder Decision")
            approved = final["state"] == "approved" or chain.get("approved_status", "APPROVED") in final["text"].upper()
            score_text = "\n".join(r["text"] for r in by_kind.get("scorecard", []))
            if approved and any(re.search(rf"\b{re.escape(value)}\b", score_text, re.I)
                                for value in chain.get("blocking_stop_gates", [])):
                errors.append(f"{decision_id}: APPROVED forbidden by blocking stop-gate")


def _validate_repository_controls(root: Path, manifest: dict, errors: list[str]) -> None:
    policy = manifest.get("repository_protection", {})
    protection_doc = root / "docs/studio/repository-protection-setup.md"
    if protection_doc.is_file():
        doc = _read(protection_doc, errors, "docs/studio/repository-protection-setup.md")
        expected_doc_values = {
            "workflow file": policy.get("required_workflow_file", ""),
            "workflow name": policy.get("required_workflow_name", ""),
            "required job ID": policy.get("required_job_id", ""),
            "job display name": policy.get("required_job_display_name", ""),
            "required status check": policy.get("required_status_check", ""),
        }
        for label, value in expected_doc_values.items():
            if value and f"{label}: `{value}`" not in doc:
                errors.append(f"Repository protection document disagrees with manifest: {label}: {value}")
    owner = policy.get("founder_github_owner")
    codeowners = root / ".github/CODEOWNERS"
    if codeowners.is_file() and owner:
        rules: dict[str, list[str]] = {}
        for line_no, raw in enumerate(_read(codeowners, errors, ".github/CODEOWNERS").splitlines(), 1):
            line = raw.split("#", 1)[0].strip()
            if not line:
                continue
            fields = line.split()
            pattern, owners = fields[0], fields[1:]
            if not owners:
                errors.append(f"CODEOWNERS:{line_no}: owner missing")
                continue
            if pattern != "*" and (not pattern.startswith("/") or ".." in pattern or "\\" in pattern):
                errors.append(f"CODEOWNERS:{line_no}: invalid or ineffective pattern: {pattern}")
                continue
            if any(not item.startswith("@") or len(item) == 1 for item in owners):
                errors.append(f"CODEOWNERS:{line_no}: invalid owner")
            rules[pattern] = owners
        if owner not in {item for owners in rules.values() for item in owners}:
            errors.append(f"CODEOWNERS: founder owner missing: {owner}")
        if policy.get("require_global_owner_rule") and owner not in rules.get("*", []):
            errors.append(f"CODEOWNERS: global '*' rule must be owned by {owner}")
        if policy.get("require_explicit_critical_patterns"):
            for pattern in policy.get("critical_codeowners_patterns", []):
                if owner not in rules.get(pattern, []):
                    errors.append(f"CODEOWNERS: explicit critical pattern missing or wrong owner: {pattern}")

    workflow_rel = policy.get("required_workflow_file", "")
    workflow = root / workflow_rel
    if not workflow.is_file():
        return
    raw = _read(workflow, errors, workflow_rel)
    active = [line for line in raw.splitlines() if line.strip() and not line.lstrip().startswith("#")]
    if not active:
        errors.append("Workflow is empty or comments only")
        return
    if any("\t" in line for line in active):
        errors.append("Workflow uses unsupported YAML tabs")
        return
    code_lines = [_yaml_without_comment(line) for line in active]
    text = "\n".join(line for line in code_lines if line.strip())
    unsupported_checks = (
        (r"(?m)(?:^|:)\s*[\[{]", "flow mapping or flow sequence"),
        (r"(?:^|[\s:,\-])[&][A-Za-z_][\w-]*", "YAML anchor"),
        (r"(?:^|[\s:,\-])[*][A-Za-z_][\w-]*", "YAML alias"),
        (r"(?m)^\s*<<\s*:", "YAML merge key"),
    )
    for pattern, label in unsupported_checks:
        if re.search(pattern, text):
            errors.append(f"Workflow uses unsupported YAML structure: {label}")
    if re.search(r"(?m)^\s*run:\s*[|>]", text):
        errors.append("Workflow multiline run YAML is unsupported")
    if "${{" in text:
        errors.append("Workflow uses unsupported dynamic expression in protected structure")

    expected_name = str(policy.get("required_workflow_name", ""))
    name_match = re.search(r"(?m)^name:\s*['\"]?([^'\"\n]+)['\"]?\s*$", text)
    if not name_match or name_match.group(1).strip() != expected_name:
        errors.append(f"Workflow name must be {expected_name}")
    if not re.search(r"(?m)^on:\s*$", text):
        errors.append("Workflow trigger mapping missing")
    for trigger in policy.get("required_triggers", []):
        if not re.search(rf"(?m)^\s{{2}}{re.escape(trigger)}:\s*$", text):
            errors.append(f"Workflow required trigger missing: {trigger}")
    branches_match = re.search(r"(?ms)^\s{2}push:\s*$.*?^\s{4}branches:\s*$((?:\n\s{6}-[^\n]+)+)", text)
    branches = set()
    if branches_match:
        branches = {item.strip().strip("'\"") for item in re.findall(r"(?m)^\s{6}-\s*(.+?)\s*$", branches_match.group(1))}
    for branch in policy.get("required_push_branches", []):
        if branch not in branches:
            errors.append(f"Workflow required push branch missing: {branch}")
    jobs_match = re.search(r"(?m)^jobs:\s*$", text)
    if not jobs_match:
        errors.append("Workflow has no supported job")
        return
    jobs_text = text[jobs_match.end():]
    job_matches = list(re.finditer(r"(?m)^ {2}([A-Za-z0-9_-]+):\s*$", jobs_text))
    jobs: dict[str, str] = {}
    for index, match in enumerate(job_matches):
        end = job_matches[index + 1].start() if index + 1 < len(job_matches) else len(jobs_text)
        jobs[match.group(1)] = jobs_text[match.end():end]
    required_job_id = policy.get("required_job_id", "")
    if required_job_id not in jobs:
        errors.append(f"Workflow required job ID missing: {required_job_id}")
        return
    required_job = jobs[required_job_id]
    display = policy.get("required_job_display_name", "")
    job_name = re.search(r"(?m)^ {4}name:\s*['\"]?([^'\"\n]+)['\"]?\s*$", required_job)
    if not job_name or job_name.group(1).strip() != display:
        errors.append(f"Workflow required job display name must be {display}")
    status = policy.get("required_status_check", "")
    if display != status:
        errors.append("Manifest required job display name and status check must match")
    for job_id, job_body in jobs.items():
        if job_id != required_job_id and re.search(
                rf"(?m)^ {{4}}name:\s*['\"]?{re.escape(display)}['\"]?\s*$", job_body):
            errors.append(f"Workflow contains spoofed duplicate job display name: {job_id}")
    if re.search(r"(?mi)^ {4}(?:continue-on-error|needs|if):\s*", required_job):
        errors.append("Workflow required job must not use continue-on-error, needs, or if")
    if re.search(r"(?mi)^\s{6,}(?:continue-on-error|if):\s*", required_job):
        errors.append("Workflow required job steps must not use continue-on-error or if")
    for action in policy.get("required_actions", []):
        matches = re.findall(rf"(?m)^\s+-\s+uses:\s*{re.escape(action)}[^\s]*\s*$", required_job)
        if len(matches) != 1:
            errors.append(f"Workflow required action missing or duplicated in required job: {action}")
        for job_id, job_body in jobs.items():
            if job_id != required_job_id and action in job_body:
                errors.append(f"Workflow required action must exist only in required job: {action}")
    required_runs = [match.group(1).strip().strip("'\"") for match in
                     re.finditer(r"(?m)^\s+run:\s*(.+?)\s*$", required_job)]
    all_runs = {job_id: [match.group(1).strip().strip("'\"") for match in
                         re.finditer(r"(?m)^\s+run:\s*(.+?)\s*$", body)]
                for job_id, body in jobs.items()}
    for required in policy.get("required_commands", []):
        if required_runs.count(required) != 1:
            errors.append(f"Workflow required command missing or not exact: {required}")
        for job_id, runs in all_runs.items():
            if job_id != required_job_id and required in runs:
                errors.append(f"Workflow required command must exist only in required job: {required}")
    for command in required_runs:
        for pattern in policy.get("forbidden_failure_masking_patterns", []):
            if re.search(pattern, command):
                errors.append("Workflow command masks failures")


def _yaml_without_comment(line: str) -> str:
    """Remove YAML comments while retaining quoted scalars and expressions."""
    quote: str | None = None
    escaped = False
    for index, char in enumerate(line):
        if escaped:
            escaped = False
            continue
        if char == "\\" and quote == '"':
            escaped = True
            continue
        if char in "'\"":
            if quote is None:
                quote = char
            elif quote == char:
                quote = None
        elif char == "#" and quote is None and (index == 0 or line[index - 1].isspace()):
            return line[:index].rstrip()
    return line.rstrip()


def validate_repository(root: Path = ROOT) -> tuple[list[str], list[str]]:
    root = Path(root)
    errors: list[str] = []
    warnings: list[str] = []
    manifest = _load_manifest(root, errors)
    if manifest:
        _validate_inventory(root, manifest, errors)
        _validate_profiles(root, manifest, errors, warnings)
        _validate_critical(root, manifest, errors)
        _validate_repository_controls(root, manifest, errors)
    _validate_markdown_links(root, errors)
    _validate_agents_links(root, manifest, errors)
    _validate_founder_authority(root, manifest, errors, warnings)
    _validate_decision_templates(root, manifest, errors)
    if manifest:
        _validate_complete_decisions(root, manifest, errors)
    warnings.append("Markdown heading anchor doğruluğu parser olmadan kesin doğrulanmaz")
    return errors, warnings


def validate(root: Path = ROOT) -> list[str]:
    """Backward-compatible error-only API used by callers and tests."""
    return validate_repository(root)[0]


def main() -> int:
    errors, warnings = validate_repository(ROOT)
    for warning in warnings:
        print(f"WARNING: {warning}", file=sys.stderr)
    if errors:
        print(f"STUDIO VALIDATION FAILED ({len(errors)} hata)")
        for error in errors:
            print(f"- {error}")
        return 1
    manifest = json.loads((ROOT / MANIFEST).read_text(encoding="utf-8"))
    print("STUDIO VALIDATION PASSED")
    print(f"Kontrol edilen agent profili: {manifest['agent_count']}")
    print(f"Kontrol edilen zorunlu dosya: {len(manifest['required_files'])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
