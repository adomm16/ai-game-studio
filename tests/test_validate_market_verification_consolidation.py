import csv
import tempfile
import unittest
from pathlib import Path

from scripts.validate_market_verification_consolidation import QUEUE, RESULTS, SOURCES, validate


class MarketVerificationConsolidationTests(unittest.TestCase):
    def test_repository_consolidation_passes(self):
        self.assertEqual([], validate())

    def test_duplicate_result_fails_closed(self):
        with tempfile.TemporaryDirectory() as tmp:
            target = Path(tmp) / "results.csv"
            with RESULTS.open(encoding="utf-8-sig", newline="") as handle:
                reader = csv.DictReader(handle)
                rows = list(reader)
                fields = reader.fieldnames
            rows[-1]["Counted Sample ID"] = rows[0]["Counted Sample ID"]
            with target.open("w", encoding="utf-8", newline="") as handle:
                writer = csv.DictWriter(handle, fieldnames=fields)
                writer.writeheader()
                writer.writerows(rows)
            errors = validate(results_path=target, sources_path=SOURCES, queue_path=QUEUE)
            self.assertTrue(any("unique Counted Sample" in error for error in errors))

    def test_verified_record_in_queue_fails_closed(self):
        with tempfile.TemporaryDirectory() as tmp:
            target = Path(tmp) / "queue.csv"
            with QUEUE.open(encoding="utf-8-sig", newline="") as handle:
                reader = csv.DictReader(handle)
                rows = list(reader)
                fields = reader.fieldnames
            rows[0]["Current Verification Result"] = "VERIFIED_ACTIVE"
            with target.open("w", encoding="utf-8", newline="") as handle:
                writer = csv.DictWriter(handle, fieldnames=fields)
                writer.writeheader()
                writer.writerows(rows)
            errors = validate(results_path=RESULTS, sources_path=SOURCES, queue_path=target)
            self.assertTrue(any("contains verified" in error for error in errors))


if __name__ == "__main__":
    unittest.main()
