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
REQUIRED_HEADINGS = [
    "Agent ID", "Kurumsal unvan", "Departman", "Rapor verdiği rol",
    "Denetlediği roller", "Temel misyon", "Uzmanlık alanları",
    "Profesyonel karakter", "Karar yaklaşımı", "Yetkileri",
    "Yetkisiz olduğu işlemler", "Zorunlu girdiler", "Zorunlu çıktılar",
    "Araştırma yükümlülükleri", "Kalite kontrol listesi", "Başarı ölçütleri",
    "Reddetme koşulları", "Escalation koşulları", "Çıkar çatışması kuralları",
    "Handoff formatı", "Diğer agentlarla çalışma şekli",
    "Kurucu onayı gerektiren durumlar",
]
RESERVED = [
    "Stüdyo adı", "Şirket kuruluşu", "Marka tescili başvurusu",
    "Alan adı satın alma", "Ana oyun projesinin seçimi", "Oyun adı",
    "Bütçe ve ücretli servis kullanımı", "Yatırımcı veya yayıncı anlaşmaları",
    "Personel veya yüklenici sözleşmeleri", "Public release",
    "Main branch korumalarının kaldırılması", "Büyük kapsam değişiklikleri",
    "Canlı para kazanma sistemi", "Oyuncu verisi toplayan sistemlerin açılması",
]
AGENTS_LINKS = [
    "docs/studio/constitution.md", "docs/studio/founder-rights.md",
    "docs/studio/decision-protocol.md", "docs/studio/research-policy.md",
    "docs/studio/intellectual-property-policy.md", "docs/studio/quality-gates.md",
    "docs/plans/PLANS.md", "docs/agents/index.md",
]
DECISION_FIELDS = [
    "Decision ID", "Record ID", "Author Agent ID", "Created Date",
    "Source References", "Conflict of Interest Declaration",
    "Previous Stage References", "Next Stage", "Review Target",
    "Independence Declaration",
]
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
    if value is None:
        return None
    return value.strip().strip("`").splitlines()[0].strip()


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
    for key in ("required_files", "required_directories", "required_agents",
                "critical_files", "independent_roles"):
        if not isinstance(data.get(key), list):
            errors.append(f"Manifest alanı eksik/geçersiz: {key}")
    return data


def _validate_inventory(root: Path, manifest: dict, errors: list[str]) -> None:
    for rel in manifest.get("required_files", []):
        if not (root / rel).is_file():
            errors.append(f"Eksik zorunlu dosya: {rel}")
    for rel in manifest.get("required_directories", []):
        if not (root / rel).is_dir():
            errors.append(f"Eksik zorunlu dizin: {rel}")


def _validate_profiles(root: Path, manifest: dict, errors: list[str]) -> dict[str, dict]:
    profiles: dict[str, dict] = {}
    agents_dir = root / "docs/agents"
    exclusions = set(manifest.get("agent_profile_exclusions", []))
    for path in sorted(agents_dir.glob("*.md")) if agents_dir.is_dir() else []:
        if path.name in exclusions:
            continue
        rel = path.relative_to(root).as_posix()
        text = _read(path, errors, rel)
        for heading in REQUIRED_HEADINGS:
            if _section(text, heading) is None:
                errors.append(f"{rel}: eksik başlık: {heading}")
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

    independent = set(manifest.get("independent_roles", []))
    for aid in sorted(independent):
        if aid in profiles and profiles[aid].get("reports") != "FOUNDER":
            errors.append(f"Bağımsız rol doğrudan FOUNDER'a raporlamalı: {aid}")
    for relation in manifest.get("forbidden_supervisor_relationships", []):
        supervisor, role = relation.get("supervisor"), relation.get("role")
        if supervisor in profiles and role in profiles[supervisor].get("supervises", []):
            errors.append(f"Yasak bağımsızlık ilişkisi: {supervisor} -> {role}")
    orch = profiles.get("studio-orchestrator")
    if orch:
        supervised_text = _section(orch["text"], "Denetlediği roller") or ""
        if "Tüm uzman roller" in supervised_text:
            errors.append("Studio Orchestrator tüm uzman rolleri denetleyemez")
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


def _validate_founder_authority(root: Path, errors: list[str], warnings: list[str]) -> None:
    founder_path = root / "docs/studio/founder-rights.md"
    founder = _read(founder_path, errors, "docs/studio/founder-rights.md") if founder_path.exists() else ""
    for item in RESERVED:
        if item not in founder:
            errors.append(f"Kurucu korunmuş yetkisi eksik: {item}")
    delegation = re.compile(r"(?i)(agent|orchestrator|director|lead).{0,100}(onaylayabilir|kararını verebilir|yetkilidir|devredilir)")
    for base in (root / "AGENTS.md", root / "docs/studio", root / "docs/agents"):
        paths = [base] if base.is_file() else list(base.glob("*.md")) if base.exists() else []
        for path in paths:
            if path == founder_path:
                continue
            rel = path.relative_to(root).as_posix()
            for line_no, line in enumerate(_read(path, errors, rel).splitlines(), 1):
                if any(item.casefold() in line.casefold() for item in RESERVED) and delegation.search(line):
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


def _validate_agents_links(root: Path, errors: list[str]) -> None:
    path = root / "AGENTS.md"
    text = _read(path, errors, "AGENTS.md") if path.exists() else ""
    for link in AGENTS_LINKS:
        if link not in text:
            errors.append(f"AGENTS.md zorunlu bağlantıyı içermiyor: {link}")


def _validate_decision_templates_and_records(root: Path, errors: list[str]) -> None:
    templates = root / "docs/decisions/templates"
    for path in templates.glob("*.md") if templates.is_dir() else []:
        text = _read(path, errors, path.relative_to(root).as_posix())
        for field in DECISION_FIELDS:
            if _section(text, field) is None:
                errors.append(f"{path.relative_to(root).as_posix()}: eksik karar alanı: {field}")

    records: list[dict] = []
    decisions_root = root / "docs/decisions"
    for state in ("active", "approved", "rejected"):
        for path in (decisions_root / state).rglob("*.md") if (decisions_root / state).is_dir() else []:
            text = _read(path, errors, path.relative_to(root).as_posix())
            record = {field: _scalar(text, field) for field in DECISION_FIELDS}
            if not all(record.values()):
                errors.append(f"{path.relative_to(root).as_posix()}: karar kayıt alanları eksik")
                continue
            record.update(path=path, text=text, kind=(text.splitlines()[0].lstrip("# ").strip().casefold() if text else ""))
            records.append(record)

    by_decision: dict[str, list[dict]] = {}
    for record in records:
        by_decision.setdefault(record["Decision ID"], []).append(record)
    for decision_id, group in by_decision.items():
        proposals = [r for r in group if r["kind"] == "proposal"]
        finals = [r for r in group if r["kind"] == "final decision record"]
        owners = {r["Author Agent ID"] for r in proposals}
        if finals and len(owners) < 3:
            errors.append(f"{decision_id}: en az üç farklı proposal sahibi gerekli")
        owner_by_record = {r["Record ID"]: r["Author Agent ID"] for r in proposals}
        for record in group:
            targets = [x.strip() for x in re.split(r"[,;\n]", record["Review Target"]) if x.strip() and x.strip().casefold() not in {"yok", "none"}]
            if record["kind"] in {"critique", "red-team report"}:
                if any(owner_by_record.get(target) == record["Author Agent ID"] for target in targets):
                    errors.append(f"{decision_id}: {record['kind']} yazarı proposal sahibiyle aynı")
        all_prior_ids = {r["Record ID"] for r in group if r not in finals}
        for final in finals:
            refs = {x.strip() for x in re.split(r"[,;\n]", final["Previous Stage References"]) if x.strip()}
            missing = all_prior_ids - refs
            if missing:
                errors.append(f"{decision_id}: final kayıt önceki aşamalara referans vermiyor: {', '.join(sorted(missing))}")


def validate_repository(root: Path = ROOT) -> tuple[list[str], list[str]]:
    root = Path(root)
    errors: list[str] = []
    warnings: list[str] = []
    manifest = _load_manifest(root, errors)
    if manifest:
        _validate_inventory(root, manifest, errors)
        _validate_profiles(root, manifest, errors)
        _validate_critical(root, manifest, errors)
    _validate_markdown_links(root, errors)
    _validate_agents_links(root, errors)
    _validate_founder_authority(root, errors, warnings)
    _validate_decision_templates_and_records(root, errors)
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
