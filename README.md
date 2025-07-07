# DBSOD: Density-Based Spatial Outlier Detection

Official implementation of "DBSOD: Density-Based Spatial Outlier Detection". Paper preprint is coming soon.

## Installation

_Note: the package was developed for Linux machines._

You can install package using `pip`:

```sh
pip install dbsod
```

Alternatively (for instance if you want to contribute) you may clone this repository, build `dbsod` and install it in `.venv` in editable mode:
```sh
git clone https://github.com/Kowd-PauUh/dbsod.git
cd dbsod
make install_g++
make install_eigen
make venv
make build
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

We can use `dbsod` to calculate outlierness score for each point:

```python
from dbsod import dbsod

EPS_SPACE = [0.15, 0.22]  # `eps` parameters used for calculating normalized outlierness score
MIN_PTS = 2               # minimum number of neighbors for the data point to become "core" point

# compute outlierness scores
outlierness_scores = dbsod(
    X=DATA,
    eps_space=EPS_SPACE,
    min_pts=MIN_PTS,
    metric='euclidean'
)

print(outlierness_scores)
```

The output will be: `array([0. , 0.5, 0. , 0. , 1. ])`.

Below is the visualization of this example:

![Simple Example](examples/figures/00-readme-example.png "Simple Example")

On the real-world data (check out [this example](examples/01.%20Real%20Data.ipynb)) result of `DBSOD` would look like:

![Real-World Example](examples/figures/01-real-data.png "Real-World Example")
