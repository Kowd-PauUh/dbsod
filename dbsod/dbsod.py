"""
Copyright 2025 Ivan Danylenko

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import ctypes
from importlib import resources
from typing import Literal

import numpy as np


dbsod_cpp_path = resources.files('dbsod').joinpath('dbsod.cpp.so')
dbsod_cpp = ctypes.CDLL(dbsod_cpp_path)
dbsod_cpp.dbsod.restype = ctypes.POINTER(ctypes.c_double)
dbsod_cpp.free_array.restype = None


def dbsod(
    X: np.ndarray,
    eps_space: list[float | int] | np.ndarray,
    min_pts: int,
    metric: Literal['euclidean', 'manhattan', 'cosine'] = 'cosine',
):
    """
    Calculates normalized outlierness scores using DBSOD algorithm,
    which performs density-based outlier detection across multiple
    neighborhood radii (`eps_space`).
    
    Parameters
    ----------
    X : np.ndarray
        2D data of shape (num_points, num_dims).
    eps_space : list[float | int] | np.ndarray
        List of `epsilon` values for which outliers are identified.
    min_pts : int
        Minimum number of points within `epsilon` radius required to
        consider particular point a "core" point.
    metric : Literal['euclidean', 'manhattan', 'cosine'], optional
        Metric used to calculate distance between distinct points.
        Default is "cosine".

    Returns
    -------
    np.ndarray
        1D array of normalized outlierness scores, where higher scores
        correspond to the points which are more likely to be outliers.
        Each score is within [0; 1] range.
    """
    # validate `X`
    if not isinstance(X, np.ndarray):
        raise TypeError(f'Argument `X` must be of type `np.ndarray`.')
    if X.ndim != 2:
        raise ValueError(f'Argument `X` must be a 2-dimensional array.')
    if not np.issubdtype(X.dtype, np.number):
        raise TypeError(f'Argument `X` must contain numeric data.')
    if np.isnan(X).any() or np.isinf(X).any():
        raise ValueError(f'Argument `X` must not contain NaN or infinite values.')

    # validate `eps_space`
    if not isinstance(eps_space, (list, np.ndarray)):
        raise TypeError(f'Argument `eps_space` must be of type `list` or `np.ndarray`.')
    if list(eps_space) == []:
        raise ValueError(f'Argument `eps_space` must be non-empty list.')
    if not all(isinstance(e, (float, int)) for e in eps_space):
        raise TypeError(f'All elements in `eps_space` must be float or int.')

    # validate `min_pts`
    if not isinstance(min_pts, int):
        raise TypeError(f'Argument `min_pts` must be of type `int`.')
    if min_pts <= 0:
        raise ValueError(f'Argument `min_pts` must be greater than zero.')
    if min_pts > X.shape[0]:
        raise ValueError(f'Argument `min_pts` cannot be greater than the number of samples in `X`.')

    # validate `metric`
    if metric not in ['euclidean', 'manhattan', 'cosine']:
        raise ValueError(f'Allowed values for `metric` are: ["euclidean", "manhattan", "cosine"].')

    # cast arrays to appropriate data type and obtain shapes
    X = X.astype(np.float64)
    rows, cols = X.shape
    eps_space = np.array(eps_space).astype(np.float32)
    num_eps_values = len(eps_space)

    # pointers for unsupported data types
    X_ptr = X.ravel(order='C').ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    metric_ptr = ctypes.c_char_p(metric.encode('utf-8'))
    eps_space_ptr = eps_space.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

    # compute outlierness scores using dbsod
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
