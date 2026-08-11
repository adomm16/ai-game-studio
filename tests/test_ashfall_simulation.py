from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
PROJECT = ROOT / "game" / "AshfallProvinces"
MODEL = PROJECT / "Source" / "AshfallProvinces" / "Public" / "Simulation" / "APSimulationModel.h"
CPP_TEST = PROJECT / "Tests" / "APSimulationModelTests.cpp"


class AshfallSimulationTests(unittest.TestCase):
    def test_unreal_free_model_contract_is_present(self):
        text = MODEL.read_text(encoding="utf-8")
        self.assertNotIn("CoreMinimal.h", text)
        for symbol in (
            "ProductionForTick", "CanMuster", "SetMustered", "AdvanceArmy",
            "ApplyCasualties", "ResolveCasualties",
        ):
            self.assertIn(symbol, text)

    def test_standalone_cpp_simulation(self):
        compiler = shutil.which("clang++") or shutil.which("g++")
        if compiler is None:
            self.skipTest("No standalone C++ compiler is installed in this environment")

        with tempfile.TemporaryDirectory() as tmp:
            executable = Path(tmp) / "ashfall_sim_test"
            completed = subprocess.run(
                [compiler, "-std=c++17", str(CPP_TEST), "-o", str(executable)],
                capture_output=True,
                text=True,
                check=False,
            )
            self.assertEqual(completed.returncode, 0, completed.stderr)
            run = subprocess.run([str(executable)], capture_output=True, text=True, check=False)
            self.assertEqual(run.returncode, 0, run.stderr)


if __name__ == "__main__":
    unittest.main()
