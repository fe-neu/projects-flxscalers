"""flxscalers: data scalers backed by a compiled C++ extension."""

from flxscalers.scalers import MinMaxScaler

from importlib.metadata import PackageNotFoundError, version


__all__ = ["MinMaxScaler"]

try:
    __version__ = version("flxscalers")
except PackageNotFoundError:
    __version__ = "0+unknown"
