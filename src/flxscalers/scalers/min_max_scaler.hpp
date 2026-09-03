#ifndef MINMAXSCALER_HPP
#define MINMAXSCALER_HPP

#include <string>
#include <vector>
#include <flxscalers/core/matrix.hpp>

class MinMaxScaler {
private:
    std::vector<double> minimums;
    std::vector<double> maximums;
    std::pair<double, double> feature_range; // [0]: Lower Bound, [1]: Upper Bound
    bool is_fitted;

public:
    MinMaxScaler(std::pair<double, double> feature_range);

    void fit(const Matrix& X);
    Matrix transform(const Matrix& X) const;
    Matrix fit_transform(const Matrix& X);
    Matrix inverse_transform(const Matrix& X) const;
};

#endif
