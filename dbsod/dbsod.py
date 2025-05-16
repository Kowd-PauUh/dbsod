import ctypes
from importlib import resources


dbsod_cpp_path = resources.files('dbsod.build').joinpath('dbsod.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)

__all__ = ['dbsod_cpp']
