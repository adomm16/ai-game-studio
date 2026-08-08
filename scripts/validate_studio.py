#!/usr/bin/env python3
from pathlib import Path
import re, sys

ROOT = Path(__file__).resolve().parents[1]
REQUIRED_FILES = ['AGENTS.md', 'README.md', 'studio.manifest.json', 'docs/index.md', 'docs/studio/constitution.md', 'docs/studio/founder-rights.md', 'docs/studio/governance.md', 'docs/studio/organization-chart.md', 'docs/studio/decision-protocol.md', 'docs/studio/meeting-protocol.md', 'docs/studio/escalation-policy.md', 'docs/studio/quality-gates.md', 'docs/studio/security-policy.md', 'docs/studio/intellectual-property-policy.md', 'docs/studio/research-policy.md', 'docs/studio/definition-of-done.md', 'docs/agents/studio-orchestrator.md', 'docs/agents/chief-executive-strategy.md', 'docs/agents/executive-producer.md', 'docs/agents/game-director.md', 'docs/agents/creative-director.md', 'docs/agents/technical-director.md', 'docs/agents/art-director.md', 'docs/agents/lead-game-designer.md', 'docs/agents/economy-balance-designer.md', 'docs/agents/narrative-worldbuilding-director.md', 'docs/agents/gameplay-engineering-lead.md', 'docs/agents/backend-online-systems-lead.md', 'docs/agents/devops-release-engineer.md', 'docs/agents/ui-ux-director.md', 'docs/agents/concept-art-lead.md', 'docs/agents/character-art-lead.md', 'docs/agents/environment-art-lead.md', 'docs/agents/technical-art-lead.md', 'docs/agents/animation-vfx-lead.md', 'docs/agents/audio-director.md', 'docs/agents/market-research-director.md', 'docs/agents/brand-marketing-director.md', 'docs/agents/community-localization-director.md', 'docs/agents/finance-monetization-director.md', 'docs/agents/legal-ip-risk-advisor.md', 'docs/agents/security-privacy-lead.md', 'docs/agents/quality-assurance-director.md', 'docs/agents/accessibility-ethics-reviewer.md', 'docs/agents/independent-red-team-auditor.md', 'docs/decisions/templates/decision-brief.md', 'docs/decisions/templates/research-memo.md', 'docs/decisions/templates/proposal.md', 'docs/decisions/templates/critique.md', 'docs/decisions/templates/rebuttal.md', 'docs/decisions/templates/scorecard.md', 'docs/decisions/templates/red-team-report.md', 'docs/decisions/templates/final-decision-record.md']
REQUIRED_HEADINGS = ['Agent ID', 'Kurumsal unvan', 'Departman', 'Rapor verdiği rol', 'Denetlediği roller', 'Temel misyon', 'Uzmanlık alanları', 'Profesyonel karakter', 'Karar yaklaşımı', 'Yetkileri', 'Yetkisiz olduğu işlemler', 'Zorunlu girdiler', 'Zorunlu çıktılar', 'Araştırma yükümlülükleri', 'Kalite kontrol listesi', 'Başarı ölçütleri', 'Reddetme koşulları', 'Escalation koşulları', 'Çıkar çatışması kuralları', 'Handoff formatı', 'Diğer agentlarla çalışma şekli', 'Kurucu onayı gerektiren durumlar']
RESERVED = ['Stüdyo adı', 'Şirket kuruluşu', 'Marka tescili başvurusu', 'Alan adı satın alma', 'Ana oyun projesinin seçimi', 'Oyun adı', 'Bütçe ve ücretli servis kullanımı', 'Yatırımcı veya yayıncı anlaşmaları', 'Personel veya yüklenici sözleşmeleri', 'Public release', 'Main branch korumalarının kaldırılması', 'Büyük kapsam değişiklikleri', 'Canlı para kazanma sistemi', 'Oyuncu verisi toplayan sistemlerin açılması']
CRITICAL = ["AGENTS.md", "docs/studio/constitution.md", "docs/studio/founder-rights.md", "docs/studio/decision-protocol.md", "docs/studio/security-policy.md"]

def validate(root=ROOT):
    root=Path(root); errors=[]
    for rel in REQUIRED_FILES:
        if not (root/rel).is_file(): errors.append(f"Eksik zorunlu dosya: {rel}")
    profiles={}; ids={}
    for path in sorted((root/"docs/agents").glob("*.md")) if (root/"docs/agents").exists() else []:
        if path.name == "index.md": continue
        text=path.read_text(encoding="utf-8")
        for heading in REQUIRED_HEADINGS:
            if f"## {heading}" not in text: errors.append(f"{path.relative_to(root)}: eksik başlık: {heading}")
        match=re.search(r"## Agent ID\s+`([^`]+)`", text)
        if not match: errors.append(f"{path.relative_to(root)}: Agent ID okunamadı"); continue
        aid=match.group(1)
        if aid in ids: errors.append(f"Tekrarlanan Agent ID: {aid}")
        ids[aid]=path; profiles[aid]=text
    for aid,text in profiles.items():
        match=re.search(r"## Rapor verdiği rol\s+`([^`]+)`", text)
        if not match: errors.append(f"{ids[aid].relative_to(root)}: raporlama rolü okunamadı")
        elif match.group(1) != "FOUNDER" and match.group(1) not in ids: errors.append(f"{ids[aid].relative_to(root)}: mevcut olmayan raporlama rolü: {match.group(1)}")
    link_re=re.compile(r"\[[^]]+\]\(([^)]+)\)")
    for path in root.rglob("*.md"):
        text=path.read_text(encoding="utf-8")
        for link in link_re.findall(text):
            if link.startswith(("http://","https://","#","mailto:")): continue
            target=(path.parent/link.split("#",1)[0]).resolve()
            if not target.exists(): errors.append(f"{path.relative_to(root)}: bozuk iç bağlantı: {link}")
    agents=(root/"AGENTS.md").read_text(encoding="utf-8") if (root/"AGENTS.md").exists() else ""
    for link in ["docs/studio/constitution.md","docs/studio/founder-rights.md","docs/studio/decision-protocol.md","docs/studio/research-policy.md","docs/plans/PLANS.md","docs/agents/index.md"]:
        if link not in agents: errors.append(f"AGENTS.md zorunlu bağlantıyı içermiyor: {link}")
    founder=(root/"docs/studio/founder-rights.md").read_text(encoding="utf-8") if (root/"docs/studio/founder-rights.md").exists() else ""
    for item in RESERVED:
        if item not in founder: errors.append(f"Kurucu korunmuş yetkisi eksik: {item}")
    for rel in CRITICAL:
        path=root/rel
        if path.exists():
            text=path.read_text(encoding="utf-8").strip()
            if len(text)<120 or re.fullmatch(r"(?is).*(todo|tbd|placeholder).*", text): errors.append(f"Kritik belge boş/yetersiz/placeholder: {rel}")
    return errors

def main():
    errors=validate()
    if errors:
        print(f"STUDIO VALIDATION FAILED ({len(errors)} hata)")
        for e in errors: print(f"- {e}")
        return 1
    print("STUDIO VALIDATION PASSED")
    print(f"Kontrol edilen agent profili: {len(list((ROOT/'docs/agents').glob('*.md')))-1}")
    print(f"Kontrol edilen zorunlu dosya: {len(REQUIRED_FILES)}")
    return 0
if __name__ == "__main__": raise SystemExit(main())
