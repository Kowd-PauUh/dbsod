import pytest
import numpy as np

from dbsod import dbsod


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
METRIC = 'euclidean'
EXPECTED_RESULT = np.array([0.0, 0.5, 0.0, 0.0, 1.0])


def test_dbsod_correctness():
    result = dbsod(
        X=DATA,
        eps_space=EPS_SPACE,
        min_pts=MIN_PTS,
        metric=METRIC
    )
    np.testing.assert_allclose(result, EXPECTED_RESULT, atol=1e-6)
