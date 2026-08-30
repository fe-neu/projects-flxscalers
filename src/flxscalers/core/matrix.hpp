#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <string>


class Matrix {
private:
    std::vector<double> data;
    
public:
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(std::size_t rows, std::size_t cols, std::vector<double> data);
    
    std::size_t rows, cols;

    double& at(std::size_t i, std::size_t j);
    double at(std::size_t i, std::size_t j) const;

    const std::vector<double>& raw() const;
    std::string repr() const;
};

#endif
