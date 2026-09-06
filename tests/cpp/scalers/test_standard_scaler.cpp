#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <cmath>
#include <stdexcept>

#include "flxscalers/core/matrix.hpp"
#include "flxscalers/scalers/standard_scaler.hpp"

using Catch::Approx;

TEST_CASE("transform() before fit() throws", "[standard]") {
    StandardScaler scaler(true, true);
    Matrix X(2, 1, {1.0, 2.0});

    CHECK_THROWS_AS(scaler.transform(X), std::logic_error);
}

TEST_CASE("inverse_transform() before fit() throws", "[standard]") {
    StandardScaler scaler(true, true);
    Matrix X(2, 1, {1.0, 2.0});

    CHECK_THROWS_AS(scaler.inverse_transform(X), std::logic_error);
}

TEST_CASE("fit_transform() centers to zero mean and scales to unit variance", "[standard]") {
    StandardScaler scaler(true, true);
    Matrix X(3, 1, {0.0, 5.0, 10.0});  // mean 5, population std sqrt(50/3)

    Matrix out = scaler.fit_transform(X);

    REQUIRE(out.rows == 3);
    REQUIRE(out.cols == 1);
    CHECK(out.at(0, 0) == Approx(-1.2247448714));
    CHECK(out.at(1, 0) == Approx(0.0));
    CHECK(out.at(2, 0) == Approx(1.2247448714));

    // The transformed column itself has mean 0 and population std 1.
    double mean = (out.at(0, 0) + out.at(1, 0) + out.at(2, 0)) / 3.0;
    double var = (std::pow(out.at(0, 0) - mean, 2.0) +
                  std::pow(out.at(1, 0) - mean, 2.0) +
                  std::pow(out.at(2, 0) - mean, 2.0)) / 3.0;
    CHECK(mean == Approx(0.0).margin(1e-12));
    CHECK(var == Approx(1.0));
}

TEST_CASE("each column is standardized independently", "[standard]") {
    StandardScaler scaler(true, true);
    // col 0: {1, 3} -> mean 2, std 1;  col 1: {10, 30} -> mean 20, std 10
    Matrix X(2, 2, {1.0, 10.0,
                    3.0, 30.0});

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(-1.0));
    CHECK(out.at(0, 1) == Approx(-1.0));
    CHECK(out.at(1, 0) == Approx(1.0));
    CHECK(out.at(1, 1) == Approx(1.0));
}

TEST_CASE("with_mean = false divides by the std without centering", "[standard]") {
    StandardScaler scaler(false, true);
    Matrix X(2, 1, {0.0, 4.0});  // mean 2, std 2

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(0.0));
    CHECK(out.at(1, 0) == Approx(2.0));
}

TEST_CASE("with_std = false centers without scaling", "[standard]") {
    StandardScaler scaler(true, false);
    Matrix X(3, 1, {1.0, 2.0, 3.0});  // mean 2

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(-1.0));
    CHECK(out.at(1, 0) == Approx(0.0));
    CHECK(out.at(2, 0) == Approx(1.0));
}

TEST_CASE("with_mean = false and with_std = false is an identity transform", "[standard]") {
    StandardScaler scaler(false, false);
    Matrix X(2, 2, {1.0, 2.0,
                    3.0, 4.0});

    Matrix out = scaler.fit_transform(X);

    for (std::size_t i = 0; i < X.rows; ++i)
        for (std::size_t j = 0; j < X.cols; ++j)
            CHECK(out.at(i, j) == Approx(X.at(i, j)));
}

TEST_CASE("a constant column does not divide by zero", "[standard]") {
    StandardScaler scaler(true, true);
    Matrix X(3, 1, {7.0, 7.0, 7.0});  // std 0 -> treated as 1

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == 0.0);
    CHECK(out.at(1, 0) == 0.0);
    CHECK(out.at(2, 0) == 0.0);
}

TEST_CASE("a constant column with with_mean = false passes the values through", "[standard]") {
    StandardScaler scaler(false, true);
    Matrix X(3, 1, {7.0, 7.0, 7.0});  // std 0 -> treated as 1, no centering

    Matrix out = scaler.fit_transform(X);

    CHECK(out.at(0, 0) == Approx(7.0));
    CHECK(out.at(1, 0) == Approx(7.0));
    CHECK(out.at(2, 0) == Approx(7.0));
}

TEST_CASE("inverse_transform() undoes transform()", "[standard]") {
    StandardScaler scaler(true, true);
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

TEST_CASE("inverse_transform() round-trips a constant column", "[standard]") {
    StandardScaler scaler(true, true);
    Matrix X(3, 1, {7.0, 7.0, 7.0});

    Matrix roundtrip = scaler.inverse_transform(scaler.fit_transform(X));

    CHECK(roundtrip.at(0, 0) == Approx(7.0));
    CHECK(roundtrip.at(1, 0) == Approx(7.0));
    CHECK(roundtrip.at(2, 0) == Approx(7.0));
}

TEST_CASE("transform() applies the statistics captured at fit() time", "[standard]") {
    StandardScaler scaler(true, true);
    scaler.fit(Matrix(2, 1, {0.0, 10.0}));  // mean = 5, std = 5

    // A separate matrix is scaled with the fitted mean/std, so values outside
    // the fitted spread extrapolate rather than being re-centered.
    Matrix out = scaler.transform(Matrix(3, 1, {5.0, 15.0, -5.0}));

    CHECK(out.at(0, 0) == Approx(0.0));
    CHECK(out.at(1, 0) == Approx(2.0));
    CHECK(out.at(2, 0) == Approx(-2.0));
}

TEST_CASE("fit_transform() preserves the input shape", "[standard]") {
    StandardScaler scaler(true, true);
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
