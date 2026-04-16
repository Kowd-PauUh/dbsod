[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.19557644.svg)](https://doi.org/10.5281/zenodo.19557644)

# DBSOD: Density-Based Spatial Outlier Detection

Official implementation of "DBSOD: Density-Based Spatial Outlier Detection". Paper preprint is coming soon.

## Algorithm

While DBSCAN is a widely used clustering algorithm, it only provides a binary label for outliers and does not assign a continuous outlierness score. To address this limitation, we propose DBSOD, a density-based spatial outlier detection method inspired by DBSCAN. The algorithm estimates the consistency with which a data point is identified as an outlier across a range of neighborhood sizes:

![DBSOD Algorithm](https://raw.githubusercontent.com/Kowd-PauUh/dbsod/main/paper/algorithm.png "DBSOD Algorithm")

The algorithm systematically varies the neighborhood size parameter $\epsilon$, evaluating outlierness across multiple density assumptions. By aggregating binary outlier classifications across these scales, it produces a normalized outlierness score for each point, reflecting how consistently the point is identified as an outlier.

## Installation

_Note: the package was developed for Linux (manylinux, x86_64) machines._

You can install package using `pip`:

```sh
pip install dbsod
```

Alternatively (for instance if you want to contribute) you may clone this repository, build `dbsod` and install it in `.venv` in editable mode:
```sh
git clone https://github.com/Kowd-PauUh/dbsod.git
cd dbsod
make install_g++
make venv
```

## Usage

Take as an example this dataset:

```python
import numpy as np

DATA = np.array([
    [0.35, 0.18],
    [0.60, 0.16],
    [0.40, 0.18],
    [0.40, 0.30],
    [0.30, 0.70],
])
```

We can use `DBSOD` to calculate outlierness score for each point:

```python
from dbsod import dbsod, DBSOD

EPS_SPACE = [0.15, 0.22]  # `eps` parameters used for calculating normalized outlierness score
MIN_PTS = 2               # minimum number of neighbors for the data point to become "core" point

# Initialize model
model = DBSOD(
    eps_space=EPS_SPACE,
    min_pts=MIN_PTS,
)

# Fit model and get outlierness scores of the training data points
# This can be also achieved using:
#     `model.fit_predict(X=DATA)` or `dbsod(X=DATA, eps_space=EPS_SPACE, min_pts=MIN_PTS)`
outlierness_scores = model.fit(X=DATA).outlierness_score
print(outlierness_scores)
```

The output will be: `array([0. , 0.5, 0. , 0. , 1. ])`.

Below is the visualization of this example:

<p align="center">
  <img src="https://raw.githubusercontent.com/Kowd-PauUh/dbsod/main/examples/figures/00-readme-example.png" width="500" />
</p>

Having fitted the `DBSOD`, we can estimate outlierness scores for the new data points:

```python
# prepare a 3x3 grid of points
x, y = np.meshgrid(np.linspace(0.2, 0.5, 3), np.linspace(0.2, 0.5, 3))
points = np.column_stack([x.ravel(), y.ravel()])

# estimate outlierness score for each new point
predictions = model.predict(points)
print(predictions)
```

The output of this is: `array([0.5, 0. , 0. , 0.5, 0. , 0. , 1. , 0.5, 1. ])`.

Notice how we can use `DBSOD` to estimate outlierness of the points in a grid with an arbitrary resolution. This effectively allows us to create outlierness heatmaps.

On the real-world data (check out [this](https://github.com/Kowd-PauUh/dbsod/blob/main/examples/01.%20Real%20Data.ipynb) and [this](https://github.com/Kowd-PauUh/dbsod/blob/main/examples/02.%20Predicting%20on%20New%20Data.ipynb) examples) result of applying `DBSOD` would look like this:

<p align="center">
  <img src="https://raw.githubusercontent.com/Kowd-PauUh/dbsod/main/examples/figures/01-real-data.png" width="350" />
  <img src="https://raw.githubusercontent.com/Kowd-PauUh/dbsod/main/examples/figures/02-predicting-on-new-data.png" width="350" /> 
</p>

## Citation

```bibtex
@software{danylenko2025dbsod, 
  author = {Danylenko, Ivan},
  doi = {10.5281/zenodo.19557644},
  month = apr,
  title = {{DBSOD: Density-Based Spatial Outlier Detection}},
  url = {https://github.com/Kowd-PauUh/dbsod},
  version = {0.1.0},
  year = {2026}
}
```
