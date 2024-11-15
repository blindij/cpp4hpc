//
// Created by Bjørn Lindi on 11/11/2024.
//

#ifndef CPP4HPC_VECTOR_H
#define CPP4HPC_VECTOR_H
#include <cstddef>
#include <vector>

namespace cpp4hpc {
    class Vector {
    public:
        Vector(size_t M) : num_rows_(M), storage_(num_rows_) {}
              double& operator()(size_t i) { return storage_[i]; }
        const double& operator()(size_t i) const { return storage_[i]; }

        size_t num_rows() const { return num_rows_; }
        static Vector add(Vector& x, Vector& y){
            Vector z(x.num_rows());
            for (auto i = 0; i < z.num_rows(); i++ ){
                z(i) = x(i) + y(i);
            }
            return z;
        }

        static void add(Vector& x, Vector& y, Vector& z){
            for (auto i = 0; i < x.num_rows(); i++){
                z(i) = x(i) + y(i);
            }
        }

    private:
        size_t              num_rows_;
        std::vector<double> storage_;
    };
}

#endif //CPP4HPC_VECTOR_H
