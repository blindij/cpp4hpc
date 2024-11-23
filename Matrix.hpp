//
// Created by Bjørn Lindi on 22/11/2024.
//
#ifndef CPP4HPC_MATRIX_H
#define CPP4HPC_MATRIX_H
#include <vector>

namespace cpp4hpc {
    class Matrix {
    public:
        Matrix(size_t M, size_t N) : num_rows_(M), num_cols_(N), storage_(num_rows_ * num_cols_) {}
              double& operator()(size_t i, size_t j)       { return storage_[i * num_cols_ + j]; }
        const double& operator()(size_t i, size_t j) const { return storage_[i * num_cols_ + j]; }

        size_t num_rows() const { return num_rows_; }
        size_t num_cols() const { return num_cols_; }
    private:
        size_t num_rows_, num_cols_;
        std::vector<double> storage_;

    };

}
#endif // CPP4HPC_MATRIX_H