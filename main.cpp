#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>
#include "Vector.h"

// using namespace cpp4hpc;
using namespace std::chrono;

cpp4hpc::Vector operator+(const cpp4hpc::Vector& x, const cpp4hpc::Vector& y){
    cpp4hpc::Vector z(x.num_rows());
    for (auto i = 0; i < z.num_rows(); i++){
        z(i) = x(i) + y(i);
    }
    return z;
}

cpp4hpc::Vector operator*(const double& a, const cpp4hpc::Vector& x) {
    cpp4hpc::Vector y(x.num_rows());
    for (size_t i = 0; i < x.num_rows(); ++i){
        y(i) = a * x(i);
    }
    return y;
}

cpp4hpc::Vector& operator+=(cpp4hpc::Vector& y, const cpp4hpc::Vector& x){
    for (size_t i = 0; i < x.num_rows(); ++i){
        y(i) += x(i);
    }
    return y;
}
int main(int argc, char* argv[]) {
    int iter = 10;
    size_t size = 1024 * 1024*128;
    double const a = 3.14159;

    // Read command line arguments
    for (int i = 0; i < argc; i++){
        if ( ( strcmp( argv[i], "-S" ) == 0 ) || ( strcmp( argv[i], "-Size") == 0 ) ){
            size = atoi( argv[++i]);
            std::cout << "Vector size is: " << static_cast<float>(size * 8/(1024*1024*1024)) << " Gigabytes." << std::endl;
        }
        else if ( ( strcmp( argv[i], "-I" ) == 0 ) || (strcmp( argv[i], "-Iterations") == 0) ){
            iter = atoi( argv[++i]);
            std::cout << "The number of iterations is: " << iter << std::endl;
        }
        else if ( (strcmp( argv[i], "-h" ) == 0 ) ||  ( strcmp( argv[i], "-help" ) == 0 ) ){
            std::cout << "  -Size (-S) <int>:        number of elements in the vector." << std::endl;
            std::cout << "  -Iterations (-I) <int>:  number of iterations." << std::endl;
            std::cout << "  -help (-h):              print this message." << std::endl;
            exit( 1 );
        }
    }

    cpp4hpc::Vector x(size), y(size);
    // Generate Random values
    auto f = []()-> double{ return std::rand() % 10000; };
    // Fill up the vectors
    // generate(x.begin(), x.end(), f);
    // generate(y.begin(), y.end(), f);
    for (auto i = 0; i < size; i++){
        y(i) = f();
        x(i) = f();
    }

    // cpp4hpc::Vector z = cpp4hpc::Vector::add(x, y);
    // cpp4hpc::Vector w(size);
    // cpp4hpc::Vector::add(x, y, w);
    // cpp4hpc::Vector u = x + y;
    auto start = high_resolution_clock::now();
    // call to daxpy function
    for (size_t j = 0; j < iter; ++j){
        y += a * x;
    }
    auto stop = high_resolution_clock::now();
    // Subtract stop and start timepoints and cast it to required unit.
    // Predefined units are nanoseconds, microseconds, milliseconds,
    // seconds, minutes, hours. Use duration_cast() function
    auto duration_ms = duration_cast<microseconds>(stop - start);
    auto duration_sec = duration_cast<seconds>(stop - start);


    // To get the value of duration use the count() member function on the duration object
    std::cout << "The execution time of the daxpy() is: " << duration_ms.count() << " microseconds." << std::endl;
    std::cout << "The execution time of the daxpy() is: " << duration_sec.count() << " seconds." << std::endl;

    return 0;
}
