#include "outliers.h"
#include <vector>

std::vector<int> outliers(
    std::vector<std::vector<std::pair<int, float>>>& neighbors,
    int minPts,
    float eps
) {
    const int N = neighbors.size();
    std::vector<int> labels(N, 1);  // 1 means outlier/noise
    std::vector<bool> core(N, false);

    // find core points
    for (int i = 0; i < N; i++) {
        // count neighbors
        int neighbors_cnt = 0;
        int max_j = neighbors[i].size();
        for (int j = 0; j < max_j; j++) {
            if (neighbors[i][j].second <= eps) {
                neighbors_cnt += 1;
            }
        }

        if (neighbors_cnt >= minPts) {
            core[i] = true;
        }
    }

    // expand clusters
    for (int i = 0; i < N; i++) {
        if (!core[i]) continue;  // skip non-core points

        labels[i] = 0;  // not an outlier/noise
        int max_j = neighbors[i].size();
        for (int j = 0; j < max_j; j++) {
            if (neighbors[i][j].second <= eps) {
                int index = neighbors[i][j].first;
                labels[index] = 0;  // not an outlier/noise
            }
        }
    }

    return labels;
}
