import csv
import shutil
import tempfile
import unittest
from pathlib import Path

from scripts.validate_market_verification_partitions import PARTITION_DIR, validate


class MarketVerificationPartitionValidatorTests(unittest.TestCase):
    def copy_partitions(self, target: Path) -> None:
        shutil.copytree(PARTITION_DIR, target)

    def mutate(self, directory: Path, batch: str, mutation) -> None:
        path = directory / f"DEC-STUDIO-NAME-001-MV-{batch}.csv"
        with path.open(encoding="utf-8-sig", newline="") as handle:
            reader = csv.DictReader(handle)
            rows = list(reader)
            fields = reader.fieldnames
        mutation(rows)
        with path.open("w", encoding="utf-8-sig", newline="") as handle:
            writer = csv.DictWriter(handle, fieldnames=fields)
            writer.writeheader()
            writer.writerows(rows)

    def test_repository_partitions_pass(self):
        self.assertEqual([], validate())

    def test_duplicate_counted_and_missing_id_are_detected(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp) / "partitions"
            self.copy_partitions(directory)
            self.mutate(directory, "B", lambda rows: rows[0].update(rows[1]))
            errors = validate(partition_dir=directory)
            self.assertTrue(any("duplicate Counted" in error for error in errors))
            self.assertTrue(any("missing Counted" in error for error in errors))

    def test_wrong_partition_membership_is_detected(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp) / "partitions"
            self.copy_partitions(directory)
            self.mutate(directory, "A", lambda rows: rows.reverse())
            errors = validate(partition_dir=directory)
            self.assertTrue(any("deterministic partition" in error for error in errors))

    def test_non_initial_verification_status_is_detected(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp) / "partitions"
            self.copy_partitions(directory)
            self.mutate(directory, "G", lambda rows: rows[0].update({"Verification Status": "REVIEWED"}))
            errors = validate(partition_dir=directory)
            self.assertTrue(any("must be NOT_STARTED" in error for error in errors))

    def test_historical_record_cannot_enter_active_partition(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp) / "partitions"
            self.copy_partitions(directory)
            historical = directory / "DEC-STUDIO-NAME-001-MV-G.csv"
            with historical.open(encoding="utf-8-sig", newline="") as handle:
                historical_row = next(csv.DictReader(handle))
            def replace(rows):
                for field in rows[0]:
                    if field in historical_row:
                        rows[0][field] = historical_row[field]
            self.mutate(directory, "A", replace)
            errors = validate(partition_dir=directory)
            self.assertTrue(any("historical record in active partition" in error for error in errors))


if __name__ == "__main__":
    unittest.main()
