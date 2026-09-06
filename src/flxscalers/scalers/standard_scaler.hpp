#ifndef STANDARDSCALER_HPP
#define STANDARDSCALER_HPP

#include <string>
#include <vector>
#include <flxscalers/core/matrix.hpp>

class StandardScaler {
private:
    std::vector<double> means;
    std::vector<double> stds;
    bool with_mean;
    bool with_std;
    bool is_fitted;

    double compute_mean_for_col(const Matrix& X, std::size_t n_col) const;
    double compute_std_for_col(const Matrix& X, std::size_t n_col, double mean) const;
public:
    StandardScaler(bool with_mean, bool with_std);

    void fit(const Matrix& X);
    Matrix transform(const Matrix& X) const;
    Matrix fit_transform(const Matrix& X);
    Matrix inverse_transform(const Matrix& X) const;
};

#endif
