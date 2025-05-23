import ctypes
from importlib import resources
from typing import Literal

import numpy as np


dbsod_cpp_path = resources.files('dbsod.build').joinpath('dbsod.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)
dbsod_cpp.dbsod.restype = ctypes.POINTER(ctypes.c_float)
dbsod_cpp.free_array.restype = None


def dbsod(
    X: np.ndarray,
    eps_space: list[float] | np.ndarray,
    min_pts: int,
    metric: Literal['euclidean', 'manhattan', 'cosine'] = 'cosine',
):
    # cast arrays to appropriate data type and obtain shapes
    X = X.astype(np.float64)
    rows, cols = X.shape
    eps_space = np.array(eps_space).astype(np.float32)
    num_eps_values = len(eps_space)

    # pointers for unsupported data types
    X_ptr = X.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    metric_ptr = ctypes.c_char_p(metric.encode('utf-8'))
    eps_space_ptr = eps_space.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

    # compute outlierness factors using dbsod
    result_ptr = dbsod_cpp.dbsod(
        X_ptr,
        rows,
        cols,
        metric_ptr,
        eps_space_ptr,
        num_eps_values,
        min_pts,
    )
    result = np.ctypeslib.as_array(result_ptr, shape=(rows,)).copy()
    dbsod_cpp.free_array(result_ptr)

    return result
