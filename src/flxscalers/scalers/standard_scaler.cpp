#include <stdexcept>
#include <cmath>

#include "standard_scaler.hpp"

StandardScaler::StandardScaler(bool with_mean, bool with_std)
    : with_mean(with_mean), with_std(with_std), is_fitted(false) {}

void StandardScaler::fit(const Matrix& X){
    // Nothing to compute since transforming will simply use mean = 0 and std = 1
    if (!with_mean && !with_std) {
        this->is_fitted = true;
        return;
    }

    means.assign(X.cols, 0.0);

    if (with_std) { stds.assign(X.cols, 0.0); }

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){

        double mean = compute_mean_for_col(X, n_col);
        if (with_mean) { means[n_col] = mean; }

        if (with_std) {
            double std = compute_std_for_col(X, n_col, mean);
            stds[n_col] = std;
        }
    }

    this->is_fitted = true;
}

double StandardScaler::compute_mean_for_col(const Matrix& X, std::size_t n_col) const {
    double sum = 0.0;

    for(std::size_t n_row = 0; n_row < X.rows; n_row++){
        sum += X.at(n_row, n_col);
    }

    return sum / X.rows;
}

double StandardScaler::compute_std_for_col(const Matrix& X, std::size_t n_col, double mean) const {
    double diff_sum = 0.0;

    for(std::size_t n_row = 0; n_row < X.rows; n_row++){
        diff_sum += pow(X.at(n_row, n_col) - mean, 2.0);
    }
    
    return sqrt(diff_sum / X.rows);
}

Matrix StandardScaler::transform(const Matrix& X) const {
    if (!with_mean && !with_std) { return X; } // Nothing to do

    if (!is_fitted) throw std::logic_error("call fit() first");

    Matrix result = Matrix(X.rows, X.cols);

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){

        double mean = 0.0;
        if (with_mean) {
            mean = means[n_col];
        }

        double standard_deviation = 1.0;
        if (with_std && stds[n_col] != 0.0) {
            standard_deviation = stds[n_col];
        }

        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) =
                (X.at(n_row, n_col) - mean)
                / standard_deviation;
        } 
    }

    return result;
}

Matrix StandardScaler::fit_transform(const Matrix& X) {
    fit(X);
    return transform(X);
}

Matrix StandardScaler::inverse_transform(const Matrix& X) const {
    if (!with_mean && !with_std) { return X; } // Nothing to do

    if (!is_fitted) throw std::logic_error("call fit() first");

    Matrix result = Matrix(X.rows, X.cols);

    for(std::size_t n_col = 0; n_col < X.cols; n_col++){

        double mean = 0.0;
        if (with_mean) {
            mean = means[n_col];
        }

        double standard_deviation = 1.0;
        if (with_std && stds[n_col] != 0.0) {
            standard_deviation = stds[n_col];
        }

        for(std::size_t n_row = 0; n_row < X.rows; n_row++){
            result.at(n_row, n_col) =
                (X.at(n_row, n_col) * standard_deviation)
                + mean;
        } 
    }

    return result;
}
