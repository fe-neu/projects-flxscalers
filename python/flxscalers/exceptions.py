"""Exceptions and Errors used throughout flxscalers"""

class NotFittedError(RuntimeError):
    """Raised when a scaler method needing fitted state is called before fit()."""

    def __init__(self, scaler: object) -> None:
        super().__init__(
            f"{type(scaler).__name__} is not fitted yet. Call fit() first."
        )
        self.scaler = scaler
