import importlib.util
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "validate_analytical_sample_method",
    ROOT / "scripts" / "validate_analytical_sample_method.py",
)
module = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = module
SPEC.loader.exec_module(module)


class AnalyticalSampleMethodTests(unittest.TestCase):
    def test_repository_method_valid_but_recovery_closed(self):
        self.assertEqual(
            module.validate_repository(ROOT),
            "CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE",
        )

    def test_18_18_6_6_6_6_is_rejected(self):
        counts = dict(zip(module.STRATUM_COUNTS, (18, 18, 6, 6, 6, 6)))
        with self.assertRaisesRegex(module.MethodValidationError, "target-band"):
            module.validate_stratum_distribution(counts, 60)

    def test_duplicate_join_fails(self):
        population = [
            {"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Record Type": "ACTIVE"},
            {"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Record Type": "ACTIVE"},
        ]
        register = [{"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Primary Layer": "L", "Region": "R"}]
        with self.assertRaisesRegex(module.MethodValidationError, "duplicate"):
            module.authoritative_join(population, register)

    def test_missing_join_fails(self):
        population = [{"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Record Type": "ACTIVE"}]
        with self.assertRaisesRegex(module.MethodValidationError, "missing register join"):
            module.authoritative_join(population, [])

    def test_missing_dimension_fails(self):
        population = [{"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Record Type": "ACTIVE"}]
        register = [{"Counted Sample ID": "A", "Canonical Entity ID": "CA", "Primary Layer": "", "Region": "R"}]
        with self.assertRaisesRegex(module.MethodValidationError, "missing dimension"):
            module.authoritative_join(population, register)

    def test_stale_verification_fails(self):
        with self.assertRaisesRegex(module.MethodValidationError, "STALE_RECHECK_REQUIRED"):
            module.require_fresh("2026-08-01T00:00:00+00:00", "2026-08-09T00:00:01+00:00")

    def test_seven_days_is_fresh(self):
        module.require_fresh("2026-08-01T00:00:00+00:00", "2026-08-08T00:00:00+00:00")

    def test_founder_acceptance_missing_keeps_gate_closed(self):
        self.assertEqual(
            module.recovery_gate("PENDING", False),
            "CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE",
        )
        self.assertEqual(module.recovery_gate("ACCEPTED", True), "OPEN")

    def test_historical_arbitrary_exclusion_fails(self):
        eligible = [f"H{i}" for i in range(15)]
        with self.assertRaisesRegex(module.MethodValidationError, "all-eligible"):
            module.validate_historical_all_eligible(eligible, eligible[:12], {"A": 6, "B": 6})

    def test_historical_all_eligible_passes(self):
        eligible = [f"H{i}" for i in range(15)]
        module.validate_historical_all_eligible(eligible, eligible, {"A": 7, "B": 8})

    def test_deterministic_tie_break_and_order(self):
        layer = next(iter(module.STRATUM_COUNTS))
        candidates = [
            {"Counted Sample ID": sample, "Primary Layer": layer, "Region": "Avrupa", "Recovery Priority": "P1"}
            for sample in ("C", "A", "B")
        ]
        first = module.deterministic_candidate_order(candidates, [], "f" * 64)
        second = module.deterministic_candidate_order(list(reversed(candidates)), [], "f" * 64)
        self.assertEqual(first, second)
        expected = sorted(("A", "B", "C"), key=lambda sample: module.tie_break_hash("f" * 64, sample))
        self.assertEqual(first, expected)

    def _rates(self, value_by_checkpoint):
        return {
            checkpoint: {metric: value for metric in module.PRIMARY_METRICS}
            for checkpoint, value in value_by_checkpoint.items()
        }

    def test_stability_threshold_pass(self):
        rates = self._rates({40: 20.0, 50: 27.5, 60: 30.0})
        self.assertTrue(module.stability_pass(rates, {40: 0, 50: 1, 60: 2}, 60))

    def test_stability_threshold_fail(self):
        rates = self._rates({40: 20.0, 50: 27.6, 60: 30.0})
        self.assertFalse(module.stability_pass(rates, {40: 0, 50: 1, 60: 2}, 60))

    def test_stability_missing_rate_fail(self):
        rates = self._rates({40: 20.0, 50: 22.0, 60: 24.0})
        self.assertFalse(module.stability_pass(rates, {40: 0, 50: 5.1, 60: 2}, 60))


if __name__ == "__main__":
    unittest.main()
