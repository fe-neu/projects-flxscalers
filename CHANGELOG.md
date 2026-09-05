# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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
