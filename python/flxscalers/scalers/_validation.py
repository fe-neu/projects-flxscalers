"""Input validation shared by the scaler classes.

Fitted-state checks are deliberately not part of this module: ``_core``
already enforces that and surfaces it as ``NotFittedError`` via its pybind11
exception translator, so re-checking it in Python would be redundant.
"""

from __future__ import annotations

import numpy as np
import numpy.typing as npt


def check_array(X: npt.ArrayLike) -> npt.NDArray[np.float64]:
    """Coerce ``X`` to a 2-D ``float64`` array, raising on wrong shape or non-finite values."""
    array = np.asarray(X, dtype=np.float64)
    if array.ndim == 1:
        raise ValueError(
            "expected a 2-D array, got a 1-D array instead. Reshape your data "
            "using array.reshape(-1, 1) if it has a single feature, or "
            "array.reshape(1, -1) if it has a single sample."
        )
    if array.ndim != 2:
        raise ValueError(f"expected a 2-D array, got {array.ndim}-D")
    if not np.isfinite(array).all():
        raise ValueError("input contains NaN or infinite values")
    return array


def check_n_features(X: npt.NDArray[np.float64], n_features_in_: int) -> npt.NDArray[np.float64]:
    """Check that ``X`` has the same number of features seen during fit."""
    if X.shape[1] != n_features_in_:
        raise ValueError(
            f"X has {X.shape[1]} features, but this scaler was fitted with {n_features_in_} features"
        )
    return X
