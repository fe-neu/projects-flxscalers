# flxscalers

A small package that offers a C++-based API for scaling data with scalers such
as `StandardScaler` or `MinMaxScaler`. The numeric core is a compiled C++17
extension (`flxscalers._core`); a thin pure-Python layer is the public API.

## Layout

```
src/flxscalers/
  core/             internal C++ core (Matrix); built as the static lib flxscalers_core
  scalers/          C++ scaler implementations (min_max_scaler.{hpp,cpp})
  bindings/         pybind11 layer -> the flxscalers._core extension
    _core.cpp         the single PYBIND11_MODULE
    register.hpp      register_*() declarations, one per scaler
    conversions.*     NumPy <-> Matrix marshalling (Matrix is never exposed to Python)
    scalers/          one binding .cpp per scaler
python/flxscalers/  pure-Python public API (wraps flxscalers._core)
  scalers/            _min_max_scaler.py, ...
  _core.pyi           type stub for the compiled module
  py.typed            PEP 561 marker
tests/cpp/          Catch2 tests; layout mirrors src/flxscalers/<area>/
tests/python/       pytest tests
CMakeLists.txt      builds flxscalers_core + the _core extension (+ C++ tests, opt-in)
CMakePresets.json   the "dev" preset for the C++ test build
pyproject.toml      scikit-build-core backend + project metadata
```

## Install (consumers)

```bash
pip install .
```

NumPy is pulled in as a runtime dependency. No system CMake, Ninja, or compiler
toolchain setup is required beyond a C++17 compiler — scikit-build-core fetches
CMake and Ninja into an isolated build environment automatically.

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
   activation, is not enough. For a Jupyter kernel you cannot launch from an
   activated shell, add
   `"env": {"PATH": "/abs/path/to/venv/bin:${PATH}"}` to its `kernel.json`
   instead.

If an editable checkout gets into a broken state, `rm -rf build` and re-run the
`pip install --no-build-isolation -e .` step.

## Testing

**C++ (Catch2).** Kept out of the wheel build; enabled by the `dev` preset,
which also skips the Python extension so no pybind11 needs to be in scope.
Catch2 is fetched via `FetchContent` on the first configure.

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Without presets: `cmake -S . -B build-test -DFLXSCALERS_BUILD_TESTS=ON
-DFLXSCALERS_BUILD_PYTHON=OFF && cmake --build build-test && ctest --test-dir
build-test --output-on-failure`.

**Python (pytest).**

```bash
pip install --no-build-isolation -e '.[test]'
pytest
```

## Adding a scaler

1. **C++ core** — `src/flxscalers/scalers/<name>.{hpp,cpp}`; add the `.cpp` to
   the `flxscalers_core` source list in `CMakeLists.txt`.
2. **Binding** — `src/flxscalers/bindings/scalers/<name>.cpp` defining
   `register_<name>(pybind11::module_&)`; declare it in
   `bindings/register.hpp`, call it from `bindings/_core.cpp`, and add the
   `.cpp` to `pybind11_add_module(_core ...)`.
3. **Python** — `python/flxscalers/scalers/_<name>.py` wrapping
   `flxscalers._core.<Name>` by composition; re-export it from
   `scalers/__init__.py` and the top-level `__init__.py`, and add the class to
   `_core.pyi`.
4. **Tests** — `tests/cpp/scalers/test_<name>.cpp` (add it to
   `tests/cpp/CMakeLists.txt`) and `tests/python/scalers/test_<name>.py`.
