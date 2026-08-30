# flxscalers

A small package that offers a C++-based API for scaling data with scalers such
as `StandardScaler` or `MinMaxScaler`.

## Layout

```
src/flxscalers/        C++ sources for the compiled extension (flxscalers._core)
  bindings.cpp
  scalers/
python/flxscalers/     pure-Python package (public API, wraps flxscalers._core)
  scalers/
CMakeLists.txt         builds the _core extension
pyproject.toml         scikit-build-core backend + project metadata
```

## Install (consumers)

```bash
pip install .
```

No system CMake, Ninja, or compiler toolchain setup is required beyond a C++17
compiler — scikit-build-core fetches CMake and Ninja into an isolated build
environment automatically.

## Development

```bash
python -m venv venv && source venv/bin/activate
pip install scikit-build-core pybind11 cmake ninja
pip install --no-build-isolation -e .
```

With the editable install, `pyproject.toml` sets `editable.rebuild = true`, so
editing a `.cpp`/`.hpp`/`CMakeLists.txt` triggers a recompile on the next
`import flxscalers` — no reinstall, just restart the Python process (or the
notebook kernel).

### Gotcha: editable rebuilds need a real, activated toolchain

`editable.rebuild = true` re-invokes `cmake` and `ninja` at import time. Two
things must hold, or every import after the first fails:

1. **Install with `--no-build-isolation`** (as above). A plain isolated
   `pip install -e .` records a path to CMake inside a temporary build
   environment (`/tmp/pip-build-env-.../cmake`); that directory is deleted
   after the install, so the rebuild step then fails with
   `cmake: not found` / `returned non-zero exit status 127`. Installing
   without isolation makes it use the `cmake`/`ninja` from the venv instead,
   which persist.

2. **Activate the venv** (`source venv/bin/activate`) before running Python or
   starting the notebook kernel, so `venv/bin` is on `PATH` and the import-time
   rebuild can find `cmake`. Running `venv/bin/python` directly, without
   activation, is not enough.

If an editable checkout gets into a broken state, `rm -rf build` and re-run the
`pip install --no-build-isolation -e .` step.

## Adding a scaler

Add the new `.cpp` to the `pybind11_add_module(_core ...)` source list in
`CMakeLists.txt`.
