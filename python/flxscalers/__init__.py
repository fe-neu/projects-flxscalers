"""flxscalers: data scalers backed by a compiled C++ extension."""

from flxscalers.exceptions import NotFittedError
from flxscalers.scalers import (
    MinMaxScaler,
    StandardScaler
    )

from importlib.metadata import PackageNotFoundError, version


__all__ = [
    "MinMaxScaler",
    "StandardScaler",
    "NotFittedError",
    ]

try:
    __version__ = version("flxscalers")
except PackageNotFoundError:
    __version__ = "0+unknown"
