from __future__ import annotations

import numpy as np
import numpy.typing as npt

from flxscalers import _core
from flxscalers.exceptions import NotFittedError
from flxscalers.scalers._validation import check_array, check_n_features


class MinMaxScaler:
    """Scale each feature onto a given range.

    Every column of ``X`` is linearly rescaled from its own observed
    ``[min, max]`` span onto ``feature_range``::

        X_std = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0))
        X_scaled = X_std * (high - low) + low

    where ``(low, high) = feature_range``. The per-column minimum and
    maximum are learned in :meth:`fit` and reused by :meth:`transform`, so
    values outside the fitted span map outside ``feature_range`` rather than
    being clipped.

    Parameters
    ----------
    feature_range : tuple of (float, float), default=(0.0, 1.0)
        Target ``(low, high)`` range for the transformed data.

    Examples
    --------
    >>> import numpy as np
    >>> from flxscalers import MinMaxScaler
    >>> X = np.array([[0.0, 10.0], [5.0, 20.0], [10.0, 30.0]])
    >>> MinMaxScaler().fit_transform(X)
    array([[0. , 0. ],
           [0.5, 0.5],
           [1. , 1. ]])
    """

    def __init__(self, feature_range: tuple[float, float] = (0.0, 1.0)) -> None:
        self.feature_range = feature_range
        # Composition, not inheritance: _impl is the compiled estimator.
        self._impl = _core.MinMaxScaler(feature_range)

    def fit(self, X: npt.ArrayLike) -> MinMaxScaler:
        """Compute the per-feature minimum and maximum used for scaling.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)
            Data used to compute the per-feature range.

        Returns
        -------
        self : MinMaxScaler
            The fitted scaler.
        """
        X = check_array(X)
        self._impl.fit(X)
        self.n_features_in_ = X.shape[1]
        return self

    def transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Scale ``X`` onto ``feature_range`` using the fitted range.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)

        Returns
        -------
        ndarray of shape (n_samples, n_features)
            The scaled data, as ``float64``.
        """
        X = check_array(X)
        if hasattr(self, "n_features_in_"):
            check_n_features(X, self.n_features_in_)
        try:
            return self._impl.transform(X)
        except _core.NotFittedError as e:
            raise NotFittedError(self) from e

    def fit_transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Fit to ``X``, then scale it. Equivalent to ``fit(X).transform(X)``."""
        X = check_array(X)
        out = self._impl.fit_transform(X)
        self.n_features_in_ = X.shape[1]
        return out

    def inverse_transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Map data from ``feature_range`` back to the original feature range.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)
            Data in the scaled space.

        Returns
        -------
        ndarray of shape (n_samples, n_features)
            The data in the original space, as ``float64``.
        """
        X = check_array(X)
        if hasattr(self, "n_features_in_"):
            check_n_features(X, self.n_features_in_)
        try:
            return self._impl.inverse_transform(X)
        except _core.NotFittedError as e:
            raise NotFittedError(self) from e

    def __repr__(self) -> str:
        return f"{type(self).__name__}(feature_range={self.feature_range})"
