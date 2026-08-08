#!/usr/bin/env python3
"""Fail-closed validation for the DEC-STUDIO-NAME-001 market package."""
from __future__ import annotations

import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SAMPLE = ROOT / "docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv"
SOURCE = ROOT / "docs/research/DEC-STUDIO-NAME-001-market-source-register.csv"
REVIEW = ROOT / "docs/research/DEC-STUDIO-NAME-001-market-source-review-ledger.csv"
PATTERNS = ROOT / "docs/research/DEC-STUDIO-NAME-001-market-patterns.md"

ACTIVE_LAYERS = {
    "Büyük uluslararası yayıncılar": 30,
    "Bağımsız stüdyolar": 45,
    "Mobil oyun şirketleri": 25,
    "PC/konsol stüdyoları": 25,
    "Strateji/MMO üreticileri": 25,
    "Teknoloji/yaratıcı üretim markaları": 30,
}
HISTORICAL_LAYERS = {"Başarılı rebrand örnekleri": 10, "Marka sorunu örnekleri": 10}
BAD_ACCESS = {"BROKEN_LINK", "ACCESS_BLOCKED", "RECHECK_REQUIRED"}
FORBIDDEN = ("candidate brand", "studio name proposal", "domain availability", "username availability")

def rows(path: Path):
    with path.open(encoding="utf-8-sig", newline="") as f:
        return list(csv.DictReader(f))

def validate(sample_path=SAMPLE, source_path=SOURCE, review_path=REVIEW, pattern_path=PATTERNS):
    errors=[]; samples=rows(Path(sample_path)); sources=rows(Path(source_path)); reviews=rows(Path(review_path))
    def require(condition, message):
        if not condition: errors.append(message)
    counted=[r.get("Counted Sample ID","") for r in samples]; canonical=[r.get("Canonical Entity ID","") for r in samples]
    require(len(samples)==200 and len(set(counted))==200 and all(counted), "expected 200 unique Counted Sample IDs")
    require(len(canonical)==200 and len(set(canonical))==200 and all(canonical), "expected 200 unique Canonical Entity IDs")
    require(len({r.get("Normalized Entity Name","") for r in samples})==200, "hidden canonical duplicate detected")
    layers=Counter(r.get("Primary Layer") for r in samples)
    for layer,n in {**ACTIVE_LAYERS,**HISTORICAL_LAYERS}.items(): require(layers[layer]==n,f"layer quota {layer}: expected {n}, got {layers[layer]}")
    active=[r for r in samples if r.get("Status")=="ACTIVE"]
    require(len(active)==180,"expected 180 ACTIVE records")
    require(sum(layers[x] for x in list(ACTIVE_LAYERS)[:5])==150,"expected 150 game/publisher layer records")
    require(layers["Teknoloji/yaratıcı üretim markaları"]==30,"expected 30 adjacent active records")
    require(sum(layers[x] for x in HISTORICAL_LAYERS)==20,"expected 20 historical cases")
    source_ids=[r.get("Source ID","") for r in sources]; require(len(source_ids)==len(set(source_ids)),"duplicate Source ID")
    source_by_id={r.get("Source ID"):r for r in sources}; sample_ids=set(counted)
    allocated=defaultdict(list)
    for s in sources:
        cid=s.get("Evidence Allocation","").split(":")[0]
        require(cid in sample_ids,f"orphan source allocation {s.get('Source ID')}")
        allocated[cid].append(s)
        if s.get("Qualifies as Activity Signal")=="YES":
            require(s.get("Review Status")=="REVIEWED",f"unreviewed qualifying source {s.get('Source ID')}")
            require(s.get("Accessibility Status") not in BAD_ACCESS,f"bad-access qualifying source {s.get('Source ID')}")
        require(s.get("Reviewer Agent ID") and s.get("Reviewer Agent ID")!=s.get("Agent ID"),f"reviewer independence failed {s.get('Source ID')}")
    for r in samples:
        for sid in filter(None,(x.strip() for x in r.get("Source IDs","").split(";"))): require(sid in source_by_id,f"orphan sample Source ID {sid}")
    for r in active:
        good=[s for s in allocated[r["Counted Sample ID"]] if s.get("Review Status")=="REVIEWED" and s.get("Qualifies as Activity Signal")=="YES" and s.get("Accessibility Status") not in BAD_ACCESS]
        require(len(good)>=2,f"fewer than two qualifying sources {r['Counted Sample ID']}")
        require(len({s.get("Publisher/Institution") for s in good})>=2,f"publisher independence failed {r['Counted Sample ID']}")
        require(len({s.get("Source Origin Group") for s in good})>=2,f"origin independence failed {r['Counted Sample ID']}")
        require(any(s.get("Qualifies as Primary Source")=="YES" for s in good),f"missing primary source {r['Counted Sample ID']}")
    review_ids={r.get("Source ID") for r in reviews}; require(set(source_ids)==review_ids,"source review ledger does not cover all sources")
    text=Path(pattern_path).read_text(encoding="utf-8")
    require("PATTERN_DATA_SHA256" in text,"pattern document lacks reproducibility marker")
    scan="\n".join(Path(p).read_text(encoding="utf-8",errors="replace").casefold() for p in (sample_path,source_path,pattern_path))
    for term in FORBIDDEN: require(term not in scan,f"forbidden proposal field/text: {term}")
    return errors

def main():
    errors=validate()
    if errors:
        print("MARKET LANDSCAPE VALIDATION FAILED")
        for e in errors: print("-",e)
        return 1
    print("MARKET LANDSCAPE VALIDATION PASSED")
    return 0

if __name__=="__main__": raise SystemExit(main())
