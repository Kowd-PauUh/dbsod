import ctypes
from importlib import resources
from typing import Literal

import numpy as np


dbsod_cpp_path = resources.files('dbsod.build').joinpath('dbsod.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)
dbsod_cpp.dbsod.restype = None


def dbsod(
    X: np.ndarray,
    metric: Literal['euclidean', 'manhattan', 'cosine'] = 'cosine',
):
    X = X.astype(np.float64)
    rows, cols = X.shape

    # pointers for unsupported data types
    X_ptr = X.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    metric_ptr = ctypes.c_char_p(metric.encode('utf-8'))

    return dbsod_cpp.dbsod(X_ptr, rows, cols, metric_ptr)
