import ctypes
from importlib import resources
from typing import Literal

import numpy as np


dbsod_cpp_path = resources.files('dbsod.build').joinpath('dbsod.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)
dbsod_cpp.dbsod.restype = ctypes.POINTER(ctypes.c_double)
dbsod_cpp.free_array.restype = None


def dbsod(
    X: np.ndarray,
    eps_space: list[float | int] | np.ndarray,
    min_pts: int,
    metric: Literal['euclidean', 'manhattan', 'cosine'] = 'cosine',
):
    # validate input
    if not isinstance(X, np.ndarray):
        raise TypeError(f'`X` must be of type `np.ndarray`.')
    if X.ndim != 2:
        raise ValueError(f'`X` must be a 2-dimensional array.')
    if not np.issubdtype(X.dtype, np.number):
        raise TypeError(f'`X` must contain numeric data.')
    if np.isnan(X).any() or np.isinf(X).any():
        raise ValueError(f'`X` must not contain NaN or infinite values.')
    if not isinstance(eps_space, (list, np.ndarray)):
        raise TypeError(f'`eps_space` must be of type `list` or `np.ndarray`.')
    if eps_space == []:
        raise ValueError(f'`eps_space` must be non-empty list.')
    if not all(isinstance(e, (float, int)) for e in eps_space):
        raise TypeError(f'All elements in `eps_space` must be float or int.')
    if not isinstance(min_pts, int):
        raise TypeError(f'`min_pts` must be of type `int`.')
    if min_pts <= 0:
        raise ValueError(f'`min_pts` must be greater than zero.')
    if min_pts > X.shape[0]:
        raise ValueError(f'`min_pts` cannot be greater than the number of samples in `X`.')
    if metric not in ['euclidean', 'manhattan', 'cosine']:
        raise ValueError(f'Allowed values for `metric` are: ["euclidean", "manhattan", "cosine"].')

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


__all__ = ['dbsod']
