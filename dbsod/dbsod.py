import ctypes
from importlib import resources

import numpy as np


dbsod_cpp_path = resources.files('dbsod.build').joinpath('dbsod.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)
dbsod_cpp.dbsod.restype = None


def dbsod(X: np.ndarray):
    X = X.astype(np.float64)

    X_ptr = X.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    rows, cols = X.shape
    return dbsod_cpp.dbsod(X_ptr, rows, cols)
