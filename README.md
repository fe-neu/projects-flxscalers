# flxscalers

[![PyPI](https://img.shields.io/pypi/v/flxscalers)](https://pypi.org/project/flxscalers/)

Data scalers with a compiled C++17 core and a thin, typed Python API. The
numeric work happens in an extension module (`flxscalers._core`); the public
Python layer only validates input and wraps the result.

## Features

- **`MinMaxScaler`** — linearly rescales every feature from its observed
  `[min, max]` span onto a configurable `feature_range` (default `(0.0, 1.0)`).
  Values outside the fitted span map outside the range rather than being
  clipped.
- **Familiar estimator API** — `fit`, `transform`, `fit_transform`, and
  `inverse_transform`, matching the scikit-learn method names and semantics.
- **Compiled core** — the per-feature statistics and the scaling pass run in
  C++17, not Python.
- **NumPy in, NumPy out** — accepts any array-like of shape
  `(n_samples, n_features)`; always returns a `float64` `ndarray`.
- **Typed** — ships `py.typed` and stubs, so `MinMaxScaler` is fully checkable
  under mypy/pyright.
- **Clear errors** — calling `transform` before `fit` raises
  `flxscalers.NotFittedError` with an actionable message.

More scalers (e.g. `StandardScaler`) are planned.

## Install

```bash
pip install .
```

NumPy is pulled in as a runtime dependency. No system CMake, Ninja, or compiler
setup is required beyond a C++17 compiler — scikit-build-core fetches CMake and
Ninja into an isolated build environment automatically.

## Usage

```python
import numpy as np
from flxscalers import MinMaxScaler

X = np.array([[0.0, 10.0],
              [5.0, 20.0],
              [10.0, 30.0]])

scaler = MinMaxScaler()
X_scaled = scaler.fit_transform(X)
# array([[0. , 0. ],
#        [0.5, 0.5],
#        [1. , 1. ]])

# Reuse the fitted range on new data; values beyond the fitted span
# extrapolate past feature_range instead of being clipped.
scaler.transform(np.array([[15.0, 40.0]]))
# array([[1.5, 1.5]])

# Round-trip back to the original units.
scaler.inverse_transform(X_scaled)
# array([[ 0., 10.],
#        [ 5., 20.],
#        [10., 30.]])
```

Scale onto a custom range by passing `feature_range`:

```python
scaler = MinMaxScaler(feature_range=(-1.0, 1.0))
scaler.fit_transform(X)
# array([[-1., -1.],
#        [ 0.,  0.],
#        [ 1.,  1.]])
```

Using a method that needs fitted state before calling `fit` raises:

```python
from flxscalers import MinMaxScaler, NotFittedError

try:
    MinMaxScaler().transform(X)
except NotFittedError as e:
    print(e)  # MinMaxScaler is not fitted yet. Call fit() first.
```

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
