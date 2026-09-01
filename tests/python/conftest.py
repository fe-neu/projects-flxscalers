import os
import sys

# The editable install has `editable.rebuild = true`, so importing `flxscalers`
# re-invokes cmake/ninja. Those live in the venv's bin dir (see README), which
# is not on PATH when a test runner (VS Code's discovery, a bare `pytest`) is
# launched without the venv activated. Put it there before any test imports the
# package, or collection fails with `FileNotFoundError: 'cmake'`.
_bin = os.path.dirname(sys.executable)
if _bin not in os.environ.get("PATH", "").split(os.pathsep):
    os.environ["PATH"] = _bin + os.pathsep + os.environ.get("PATH", "")
