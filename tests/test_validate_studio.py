import tempfile, unittest
from pathlib import Path
from shutil import copytree
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/"scripts"))
from validate_studio import validate, ROOT

class StudioValidatorTests(unittest.TestCase):
    def fixture(self):
        tmp=tempfile.TemporaryDirectory(); dst=Path(tmp.name)/"repo"; copytree(ROOT,dst,ignore=lambda p,n:{".git","__pycache__"}); return tmp,dst
    def test_repository_is_valid(self): self.assertEqual([],validate(ROOT))
    def test_missing_required_file(self):
        tmp,dst=self.fixture()
        try: (dst/"AGENTS.md").unlink(); self.assertTrue(any("Eksik zorunlu dosya" in e for e in validate(dst)))
        finally: tmp.cleanup()
    def test_duplicate_agent_id(self):
        tmp,dst=self.fixture()
        try:
            source=dst/"docs/agents/studio-orchestrator.md"; (dst/"docs/agents/duplicate.md").write_text(source.read_text(encoding="utf-8"),encoding="utf-8")
            self.assertTrue(any("Tekrarlanan Agent ID" in e for e in validate(dst)))
        finally: tmp.cleanup()
    def test_unknown_reporting_role(self):
        tmp,dst=self.fixture()
        try:
            path=dst/"docs/agents/studio-orchestrator.md"; text=path.read_text(encoding="utf-8").replace("`FOUNDER`","`missing-role`",1); path.write_text(text,encoding="utf-8")
            self.assertTrue(any("mevcut olmayan raporlama rolü" in e for e in validate(dst)))
        finally: tmp.cleanup()
    def test_broken_internal_link(self):
        tmp,dst=self.fixture()
        try:
            path=dst/"README.md"; path.write_text(path.read_text(encoding="utf-8")+"\n[broken](missing.md)\n",encoding="utf-8")
            self.assertTrue(any("bozuk iç bağlantı" in e for e in validate(dst)))
        finally: tmp.cleanup()
    def test_missing_founder_right(self):
        tmp,dst=self.fixture()
        try:
            path=dst/"docs/studio/founder-rights.md"; path.write_text(path.read_text(encoding="utf-8").replace("- Stüdyo adı\n",""),encoding="utf-8")
            self.assertTrue(any("Kurucu korunmuş yetkisi eksik" in e for e in validate(dst)))
        finally: tmp.cleanup()
if __name__ == "__main__": unittest.main()
