#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <vector>

#include "flxscalers/core/matrix.hpp"

TEST_CASE("Matrix(rows, cols) zero-fills every cell", "[matrix]") {
    Matrix m(2, 3);

    CHECK(m.rows == 2);
    CHECK(m.cols == 3);

    for (std::size_t i = 0; i < m.rows; ++i) {
        for (std::size_t j = 0; j < m.cols; ++j) {
            CHECK(m.at(i, j) == 0.0);
        }
    }
}

TEST_CASE("Matrix(rows, cols, data) stores elements row-major", "[matrix]") {
    Matrix m(2, 2, {1.0, 2.0, 3.0, 4.0});

    CHECK(m.at(0, 0) == 1.0);
    CHECK(m.at(0, 1) == 2.0);
    CHECK(m.at(1, 0) == 3.0);
    CHECK(m.at(1, 1) == 4.0);
}

TEST_CASE("at() returns a writable reference", "[matrix]") {
    Matrix m(2, 2);

    m.at(1, 0) = 42.0;

    CHECK(m.at(1, 0) == 42.0);
    CHECK(m.at(0, 0) == 0.0);  // neighbouring cells untouched
}

TEST_CASE("const at() reads the same layout", "[matrix]") {
    const Matrix m(2, 2, {10.0, 20.0, 30.0, 40.0});

    CHECK(m.at(0, 1) == 20.0);
    CHECK(m.at(1, 1) == 40.0);
}

TEST_CASE("raw() exposes the flat backing buffer", "[matrix]") {
    Matrix m(2, 2, {1.0, 2.0, 3.0, 4.0});

    const std::vector<double>& buf = m.raw();

    REQUIRE(buf.size() == 4);
    CHECK(buf == std::vector<double>{1.0, 2.0, 3.0, 4.0});
}

TEST_CASE("repr() separates rows with newlines", "[matrix]") {
    Matrix m(2, 2, {1.0, 2.0, 3.0, 4.0});

    CHECK(m.repr() == " 1.000000 2.000000\n 3.000000 4.000000");
}

TEST_CASE("at() rejects out-of-bounds indices", "[matrix]") {
    Matrix m(2, 3);
    const Matrix cm(2, 3);

    SECTION("row index past the last row throws") {
        CHECK_THROWS_AS(m.at(2, 0), std::out_of_range);
        CHECK_THROWS_AS(cm.at(2, 0), std::out_of_range);
    }

    SECTION("column index past the last column throws") {
        CHECK_THROWS_AS(m.at(0, 3), std::out_of_range);
        CHECK_THROWS_AS(cm.at(0, 3), std::out_of_range);
    }

    SECTION("the last valid element does not throw") {
        CHECK_NOTHROW(m.at(1, 2));
        CHECK_NOTHROW(cm.at(1, 2));
    }
}
