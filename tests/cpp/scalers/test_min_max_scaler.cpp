#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <cmath>
#include <stdexcept>

#include "flxscalers/core/matrix.hpp"
#include "flxscalers/scalers/min_max_scaler.hpp"

using Catch::Approx;

TEST_CASE("transform() before fit() throws", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(2, 1, {1.0, 2.0});

    CHECK_THROWS_AS(scaler.transform(X), std::logic_error);
}

TEST_CASE("inverse_transform() before fit() throws", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(2, 1, {1.0, 2.0});

    CHECK_THROWS_AS(scaler.inverse_transform(X), std::logic_error);
}

TEST_CASE("fit_transform() maps a column onto [0, 1]", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(3, 1, {0.0, 5.0, 10.0});

    Matrix out = scaler.fit_transform(X);

    REQUIRE(out.rows == 3);
    REQUIRE(out.cols == 1);
    CHECK(out.at(0, 0) == Approx(0.0));   // column min -> range low
    CHECK(out.at(1, 0) == Approx(0.5));   // midpoint
    CHECK(out.at(2, 0) == Approx(1.0));   // column max -> range high
}

TEST_CASE("each column is scaled independently", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    // col 0 spans [1, 3], col 1 spans [10, 30]
    Matrix X(2, 2, {1.0, 10.0,
                    3.0, 30.0});

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(0.0));
    CHECK(out.at(0, 1) == Approx(0.0));
    CHECK(out.at(1, 0) == Approx(1.0));
    CHECK(out.at(1, 1) == Approx(1.0));
}

TEST_CASE("a custom feature_range is honoured", "[minmax]") {
    MinMaxScaler scaler({-1.0, 1.0});
    Matrix X(3, 1, {0.0, 5.0, 10.0});

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(-1.0));
    CHECK(out.at(1, 0) == Approx(0.0));
    CHECK(out.at(2, 0) == Approx(1.0));
}

TEST_CASE("inverse_transform() undoes transform()", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(3, 2, {2.0, 100.0,
                    4.0, 200.0,
                    6.0, 300.0});

    Matrix roundtrip = scaler.inverse_transform(scaler.fit_transform(X));

    for (std::size_t i = 0; i < X.rows; ++i) {
        for (std::size_t j = 0; j < X.cols; ++j) {
            CHECK(roundtrip.at(i, j) == Approx(X.at(i, j)).margin(1e-9));
        }
    }
}

TEST_CASE("transform() applies the statistics captured at fit() time", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    scaler.fit(Matrix(2, 1, {0.0, 10.0}));  // min = 0, max = 10

    // A separate matrix is scaled with the fitted min/max, so values outside
    // the fitted span extrapolate past the feature range rather than clamping.
    Matrix out = scaler.transform(Matrix(3, 1, {5.0, -5.0, 20.0}));

    CHECK(out.at(0, 0) == Approx(0.5));
    CHECK(out.at(1, 0) == Approx(-0.5));
    CHECK(out.at(2, 0) == Approx(2.0));
}

TEST_CASE("fit_transform() preserves the input shape", "[minmax]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(4, 3, {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0,
        10.0, 11.0, 12.0,
    });

    Matrix out = scaler.fit_transform(X);

    CHECK(out.rows == 4);
    CHECK(out.cols == 3);
}

// Characterisation test, not a spec: a column with zero spread divides by
// (max - min) == 0. scikit-learn special-cases this to leave the column at the
// range low; this implementation currently produces NaN. Captured here so the
// behaviour change is visible when that gap is closed.
TEST_CASE("a constant column currently yields NaN", "[minmax][known-gap]") {
    MinMaxScaler scaler({0.0, 1.0});
    Matrix X(3, 1, {7.0, 7.0, 7.0});

    Matrix out = scaler.fit_transform(X);

    CHECK(std::isnan(out.at(0, 0)));
}
