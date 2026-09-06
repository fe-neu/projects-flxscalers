from __future__ import annotations

import numpy as np
import numpy.typing as npt

from flxscalers import _core
from flxscalers.exceptions import NotFittedError


class StandardScaler:
    """Standardize each feature by centering and scaling to unit variance.

    Every column of ``X`` is transformed independently::

        X_scaled = (X - X.mean(axis=0)) / X.std(axis=0)

    The per-column mean and population standard deviation (dividing by
    ``n_samples``) are learned in :meth:`fit` and reused by
    :meth:`transform`. Centering is skipped when ``with_mean`` is false and
    scaling is skipped when ``with_std`` is false; if both are false the
    data passes through unchanged.

    Parameters
    ----------
    with_mean : bool, default=True
        If true, center the data by subtracting the per-feature mean.
    with_std : bool, default=True
        If true, scale the data to unit variance by dividing by the
        per-feature standard deviation.

    Examples
    --------
    >>> import numpy as np
    >>> from flxscalers import StandardScaler
    >>> X = np.array([[0.0, 10.0], [5.0, 20.0], [10.0, 30.0]])
    >>> StandardScaler().fit_transform(X)
    array([[-1.22474487, -1.22474487],
           [ 0.        ,  0.        ],
           [ 1.22474487,  1.22474487]])
    """

    def __init__(self, with_mean: bool = True, with_std: bool = True) -> None:
        self.with_mean = with_mean
        self.with_std = with_std
        # Composition, not inheritance: _impl is the compiled estimator.
        self._impl = _core.StandardScaler(with_mean, with_std)

    def fit(self, X: npt.ArrayLike) -> StandardScaler:
        """Compute the per-feature mean and population standard deviation used for scaling.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)
            Data used to compute the per-feature mean and standard deviation.

        Returns
        -------
        self : StandardScaler
            The fitted scaler.
        """
        self._impl.fit(self._validate(X))
        return self

    def transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Center and scale ``X`` using the fitted mean and standard deviation.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)

        Returns
        -------
        ndarray of shape (n_samples, n_features)
            The standardized data, as ``float64``.
        """
        try:
            return self._impl.transform(self._validate(X))
        except RuntimeError as e:
            raise NotFittedError(self) from e

    def fit_transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Fit to ``X``, then standardize it. Equivalent to ``fit(X).transform(X)``."""
        return self._impl.fit_transform(self._validate(X))

    def inverse_transform(self, X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Undo standardization, mapping data back to the original feature space.

        Parameters
        ----------
        X : array-like of shape (n_samples, n_features)
            Data in the standardized space.

        Returns
        -------
        ndarray of shape (n_samples, n_features)
            The data in the original space, as ``float64``.
        """
        try:
            return self._impl.inverse_transform(self._validate(X))
        except RuntimeError as e:
            raise NotFittedError(self) from e

    def __repr__(self) -> str:
        return f"{type(self).__name__}(with_mean={self.with_mean}, with_std={self.with_std})"

    @staticmethod
    def _validate(X: npt.ArrayLike) -> npt.NDArray[np.float64]:
        """Coerce ``X`` to a 2-D ``float64`` array, raising on wrong ndim."""
        # TODO: 1-D handling, non-finite checks, feature-count check on transform.
        array = np.asarray(X, dtype=np.float64)
        if array.ndim != 2:
            raise ValueError(f"expected a 2-D array, got {array.ndim}-D")
        return array
