import pytest
import numpy as np

from dbsod import DBSOD, dbsod


DATA = np.array(
    [
        [0.35, 0.18],
        [0.60, 0.16],
        [0.40, 0.18],
        [0.40, 0.30],
        [0.30, 0.70],
    ]
)
EPS_SPACE = [0.15, 0.22]
MIN_PTS = 2
EXPECTED_RESULT = np.array([0.0, 0.5, 0.0, 0.0, 1.0])


def test_DBSOD_fit():
    model = DBSOD(eps_space=EPS_SPACE, min_pts=MIN_PTS)
    result = model.fit(X=DATA).outlierness_score

    assert isinstance(result, np.ndarray), "Expected result to be a numpy ndaray"
    np.testing.assert_allclose(result, EXPECTED_RESULT, atol=1e-6)


def test_DBSOD_fit_predict():
    model = DBSOD(eps_space=EPS_SPACE, min_pts=MIN_PTS)
    result = model.fit_predict(X=DATA)

    assert isinstance(result, np.ndarray), "Expected result to be a numpy ndaray"
    np.testing.assert_allclose(result, EXPECTED_RESULT, atol=1e-6)


def test_dbsod():
    result = dbsod(X=DATA, eps_space=EPS_SPACE, min_pts=MIN_PTS)
    
    assert isinstance(result, np.ndarray), "Expected result to be a numpy ndaray"
    np.testing.assert_allclose(result, EXPECTED_RESULT, atol=1e-6)
