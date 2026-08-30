#include <string>

#include "matrix.hpp"

Matrix::Matrix(std::size_t rows, std::size_t cols)
    : data(rows * cols, 0.0), rows(rows), cols(cols) {}


Matrix::Matrix(std::size_t rows, std::size_t cols, std::vector<double> data)
    : data(std::move(data)), rows(rows), cols(cols) {}


double& Matrix::at(std::size_t i, std::size_t j) {
    return data[i * cols + j];
}

double Matrix::at(std::size_t i, std::size_t j) const{
    return data[i * cols + j];
}

const std::vector<double>& Matrix::raw() const{
    return data;
}

std::string Matrix::repr() const {
    std::string output;

    for (std::size_t i = 0; i < rows * cols; ++i) {
        if (i != 0 && i % cols == 0) {
            output += "\n";
        }
        output += " " + std::to_string(data[i]);
    }

    return output;
}
