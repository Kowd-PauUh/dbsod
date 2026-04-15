#include <vector>

#include <gtest/gtest.h>

#include "dbsod.h"

std::vector<double> data = {
    0.35, 0.18,
    0.60, 0.16,
    0.40, 0.18,
    0.40, 0.30,
    0.30, 0.70
};
size_t rows = 5, cols = 2;
std::vector<double> eps_space = {0.15, 0.22};
size_t min_pts = 2;
std::vector<double> expected_result = {0.0, 0.5, 0.0, 0.0, 1.0};

void assert_size(std::vector<double> &a, std::vector<double> &b) {
    ASSERT_EQ(a.size(), b.size()) << "Result size mismatch";
}

void assert_allclose(std::vector<double> &a, std::vector<double> &b, double atol = 1e-6) {
    for (size_t i = 0; i < a.size(); ++i) {
        EXPECT_NEAR(a[i], b[i], atol) << "Mismatch at index " << i;
    }
}

// dbsod::DBSOD::fit
TEST(dbsod_functional, fit_test) {
    dbsod::DBSOD model(eps_space, min_pts);
    model.fit(data, rows, cols);

    assert_size(model.outlierness_score, expected_result);
    assert_allclose(model.outlierness_score, expected_result);
}

// dbsod::DBSOD::fit_predict
TEST(dbsod_functional, fit_predict_test) {
    dbsod::DBSOD model(eps_space, min_pts);
    auto result = model.fit_predict(data, rows, cols);

    assert_size(result, expected_result);
    assert_allclose(result, expected_result);
}

// dbsod::dbsod
TEST(dbsod_functional, dbsod_test) {
    auto result = dbsod::dbsod(data, rows, cols, eps_space, min_pts);

    assert_size(result, expected_result);
    assert_allclose(result, expected_result);
}
