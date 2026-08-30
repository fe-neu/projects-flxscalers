#include <string>
#include <stdexcept>
#include <utility>

#include "min_max_scaler.hpp"

MinMaxScaler::MinMaxScaler(std::pair<double, double> feature_range)
    : feature_range(feature_range), is_fitted(false) {}

void MinMaxScaler::fit(const Matrix& X){

    minimums.assign(X.cols, 0.0);
    maximums.assign(X.cols, 0.0);

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){
        double minimum = X.at(0, n_col);
        double maximum = X.at(0, n_col);

        for(std::size_t n_row = 1; n_row < X.rows; n_row++){
            if(X.at(n_row, n_col) < minimum){
                minimum = X.at(n_row, n_col);
            }

            if(X.at(n_row, n_col) > maximum){
                maximum = X.at(n_row, n_col);
            }
        }
        minimums[n_col] = minimum;
        maximums[n_col] = maximum;
    }

    this->is_fitted = true;
}

Matrix MinMaxScaler::transform(const Matrix& X) const {

    if (!is_fitted) throw std::logic_error("call fit() first");

    Matrix result = Matrix(X.rows, X.cols);

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){
        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) = (
                (X.at(n_row, n_col) - minimums[n_col])
                / (maximums[n_col] - minimums[n_col])
            ) * (feature_range.second - feature_range.first) + feature_range.first;
        } 
    }

    return result;
}

Matrix MinMaxScaler::fit_transform(const Matrix& X)
{
    fit(X);
    return transform(X);
}

Matrix MinMaxScaler::inverse_transform(const Matrix& X) const {
    if (!is_fitted) throw std::logic_error("call fit() first");

    Matrix result = Matrix(X.rows, X.cols);

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){
        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) = (
                (X.at(n_row, n_col) - feature_range.first)
                / (feature_range.second - feature_range.first)
            ) * (maximums[n_col] - minimums[n_col]) + minimums[n_col];
        } 
    }

    return result;
}
