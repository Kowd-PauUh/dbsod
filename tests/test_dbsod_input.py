import pytest
import numpy as np

from dbsod import dbsod


def test_invalid_X_type():
    with pytest.raises(TypeError, match='Argument `X` must be of type `np.ndarray`.'):
        dbsod("not an array", [0.5], 5)


def test_invalid_X_ndim():
    with pytest.raises(ValueError, match='Argument `X` must be a 2-dimensional array.'):
        dbsod(np.array([1, 2, 3]), [0.5], 5)


def test_invalid_X_dtype():
    with pytest.raises(TypeError, match='Argument `X` must contain numeric data.'):
        dbsod(np.array([["a", "b"], ["c", "d"]]), [0.5], 5)


def test_invalid_X_nan_inf():
    X = np.array([[1.0, np.nan], [np.inf, 2.0]])
    with pytest.raises(ValueError, match='Argument `X` must not contain NaN or infinite values.'):
        dbsod(X, [0.5], 5)


def test_invalid_eps_space_type():
    with pytest.raises(TypeError, match='Argument `eps_space` must be of type `list` or `np.ndarray`.'):
        dbsod(np.random.rand(5, 2), "not a list", 5)


def test_empty_eps_space():
    with pytest.raises(ValueError, match='Argument `eps_space` must be non-empty list.'):
        dbsod(np.random.rand(5, 2), [], 5)


def test_invalid_eps_space_element_type():
    with pytest.raises(TypeError, match='All elements in `eps_space` must be float or int.'):
        dbsod(np.random.rand(5, 2), [0.5, "invalid"], 5)


def test_invalid_min_pts_type():
    with pytest.raises(TypeError, match='Argument `min_pts` must be of type `int`.'):
        dbsod(np.random.rand(5, 2), [0.5], "5")


def test_invalid_min_pts_value():
    with pytest.raises(ValueError, match='Argument `min_pts` must be greater than zero.'):
        dbsod(np.random.rand(5, 2), [0.5], 0)


def test_min_pts_greater_than_samples():
    with pytest.raises(ValueError, match='Argument `min_pts` cannot be greater than the number of samples in `X`.'):
        dbsod(np.random.rand(3, 2), [0.5], 5)


def test_invalid_metric():
    with pytest.raises(ValueError, match=r'Allowed values for `metric` are: \["euclidean", "manhattan", "cosine"\].'):
        dbsod(np.random.rand(5, 2), [0.5], 5, metric='invalid')
