# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.0] - 2026-09-13

### Added

- `StandardScaler`: per-feature standardization to zero mean and unit
  variance, using the population standard deviation. `with_mean` and
  `with_std` toggle centering and scaling independently; zero-variance
  features are left unscaled instead of dividing by zero. Provides `fit`,
  `transform`, `fit_transform`, and `inverse_transform`.
- `n_features_in_` attribute on `MinMaxScaler` and `StandardScaler`, set by
  `fit`/`fit_transform`. `transform` and `inverse_transform` now raise
  `ValueError` if called with a different number of features.
- Scalers now reject non-finite (NaN/Inf) input with a clear `ValueError`.

### Changed

- The C++ core now throws a dedicated `NotFittedError` type, registered with
  pybind11 as `flxscalers._core.NotFittedError` via a proper exception
  translator, instead of a generic `std::runtime_error` that crossed into
  Python as a plain `RuntimeError`. The public `flxscalers.NotFittedError`
  and its message are unchanged.
- Per-scaler input validation, previously duplicated in each scaler class,
  is now centralized in `flxscalers.scalers._validation`
  (`check_array`, `check_n_features`). The 1-D rejection error now includes
  a reshape hint. Fitted-state checks remain in the C++ core.

## [0.1.0] - 2026-09-05

### Added

- Initial release.
- `MinMaxScaler`: linear per-feature rescaling from the fitted `[min, max]`
  span onto a configurable `feature_range` (default `(0.0, 1.0)`), with
  `fit`, `transform`, `fit_transform`, and `inverse_transform`. Values
  outside the fitted span extrapolate rather than being clipped.
- Compiled C++17 core (`flxscalers._core`) built via pybind11 and
  scikit-build-core; the Python layer only validates input and wraps results.
- `NotFittedError`, raised with an actionable message when a method that
  needs fitted state is called before `fit`.
- Typed public API: ships `py.typed` and stub files.

[Unreleased]: https://github.com/fe-neu/projects-flxscalers/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/fe-neu/projects-flxscalers/releases/tag/v0.1.0
[0.2.0]: https://github.com/fe-neu/projects-flxscalers/releases/tag/v0.2.0
