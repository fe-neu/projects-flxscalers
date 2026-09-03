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

    const double lower_limit = feature_range.first;
    const double feature_span = feature_range.second - lower_limit;

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){

        const double minimum = minimums[n_col];
        const double span = maximums[n_col] - minimum;

        // Checking for Constant Columns and set all to the lower limit of the feature range
        if (span == 0.0) {
            for(std::size_t n_row = 0; n_row < X.rows; n_row++){
                result.at(n_row, n_col) = lower_limit;
            }
            continue; 
        }

        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) = (
                (X.at(n_row, n_col) - minimum)
                / span
            ) * feature_span + lower_limit;
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

    const double lower_limit = feature_range.first;
    const double feature_span = feature_range.second - lower_limit;

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){

        const double minimum = minimums[n_col];
        const double span = maximums[n_col] - minimum;
        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) = (
                (X.at(n_row, n_col) - lower_limit)
                / feature_span
            ) * span + minimum;
        } 
    }

    return result;
}
