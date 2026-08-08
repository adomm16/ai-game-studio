import csv
import tempfile
import unittest
from pathlib import Path

from scripts.validate_market_landscape import validate

class MarketValidatorTests(unittest.TestCase):
    def test_repository_package_fails_closed_while_sources_need_recheck(self):
        errors=validate()
        self.assertIn("expected 180 ACTIVE records", errors)

    def test_duplicate_canonical_ids_are_detected(self):
        root=Path(__file__).resolve().parents[1]
        src=root/"docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv"
        with tempfile.TemporaryDirectory() as td:
            dst=Path(td)/"samples.csv"
            with src.open(encoding="utf-8-sig",newline="") as f: data=list(csv.DictReader(f)); fields=data[0].keys()
            data[1]["Canonical Entity ID"]=data[0]["Canonical Entity ID"]
            with dst.open("w",encoding="utf-8-sig",newline="") as f: w=csv.DictWriter(f,fieldnames=fields); w.writeheader(); w.writerows(data)
            errors=validate(dst)
            self.assertTrue(any("Canonical Entity" in e for e in errors))

if __name__=="__main__": unittest.main()
