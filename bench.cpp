//
// Created by Bjørn Lindi on 23/11/2024.
//
#include <iostream>
#include "Matrix.hpp"
#include "Timer.hpp"
#ifdef __linux__
#include "papi.h"
#endif

using cpp4hpc::Matrix;
using cpp4hpc::Timer;

#ifdef __APPLE__
auto benchmark(int M, int N, int K, long numruns){
    Matrix A(M, K), B(K, N), C(M, N);

    Timer T;
    T.start();
    for ( int i = 0; i < numruns; i++){
        C = A * B;
        //multiply(A,B,C);
    }
    T.stop();

    return T.elapsedString();
}
#endif

#ifdef __linux__
auto benchmark(int M, int N, int K, long numruns){
    // Initialize PAPI library
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
       std::cerr << "Papi library initialization error!" << std::endl;
       return 1;
    }

    // Create an Event Set
    int eventSet = PAPI_NULL;
    if (PAPI_create_eventset(&eventSet) != PAPI_OK) {
       std::cerr << "Failed to create PAPI event set!" << std::endl;
       return 1;
    }

    // Add PAPI_VEC_DP, PAPI_DP_OPS, PAPI_VEC_SP and PAPI_SP_OPS to the Event Set
    if (PAPI_add_event(eventSet, PAPI_VEC_DP) != PAPI_OK){
       std::cerr << "Failed to add PAPI_VEC_DP event!" << std::endl;
       return 1;
    }
    if (PAPI_add_event(eventSet, PAPI_DP_OPS) != PAPI_OK){
       std::cerr << "Failed to add PAPI_DP_OPS event!" << std::endl;
       return 1;
    }
    if (PAPI_add_event(eventSet, PAPI_VEC_SP) != PAPI_OK){
       std::cerr << "Failed to add PAPI_VEC_SP event!" << std::endl;
       return 1;
    }
    if (PAPI_add_event(eventSet, PAPI_SP_OPS) != PAPI_OK){
       std::cerr << "Failed to add PAPI_SP_OPS event!" << std::endl;
       return 1;
    }

    // Add PAPI_FP_OPS to the Event Set
    if (PAPI_add_event(eventSet, PAPI_FP_OPS) != PAPI_OK){
       std::cerr << "Failed to add PAPI_FP_OPS event!" << std::endl;
       // return 1;
    }
    // Start counting and record start time
    long long values[5] = {0, 0, 0, 0, 0};  // To hold counts for VEC_DP, DP, VEC_SP and SP ops and FP_OPS
    long long start_time = PAPI_get_real_usec();

    if (PAPI_start(eventSet) != PAPI_OK) {
       std::cerr << "PAPI failed to start counting!" << std::endl;
       return 1;
    }
    Matrix A(M, K), B(K, N), C(M, N);

    Timer T;
    T.start();
    for ( int i = 0; i < numruns; i++){
        C = A * B;
        //multiply(A,B,C);
    }
    T.stop();
        // Stop counting and read the results
    long long end_time = PAPI_get_real_usec();
    if (PAPI_stop(eventSet, values) != PAPI_OK){
       std::cerr << "PAPI failed to stop counting!" << std::endl;
       return 1;
    }
    auto stop = high_resolution_clock::now();

    // Calculate execution time in seconds and GFLOPS
    auto elapsed_time_sec = (end_time - start_time) / 1.0e6;
    auto total_flops = values[1] + values[3];
    auto gflops = (total_flops/1.0e9)/elapsed_time_sec;

    // Output results
    std::cout << "Vectorized double-precision operations (PAPI_VEC_DP): " << values[0] << std::endl;
    std::cout << "Total double-precision operations (PAPI_DP_OPS): " << values[1] << std::endl;
    std::cout << "Vectorized single-precision operations (PAPI_VEC_SP): " << values[2] << std::endl;
    std::cout << "Total single-precision operations (PAPI_SP_OPS): " << values[3] << std::endl;
    std::cout << "Total floating-point operations (PAPI_FP_OPS): " << values[1] + values[3] << std::endl;
    std::cout << "Total operations (PAPI_FP_OPS): " << values[4] << std::endl;
    std::cout << std::endl;
        if ( values[1] != 0)
      std::cout << "Fraction of Vectorized double-precision operations: " << (double) values[0]/values[1] << std::endl;
    if ( values[3] != 0)
      std::cout << "Fraction of Vectorize single-precision operations: " << (double) values[2]/values[3] << std::endl;
    std::cout << "Performance: " << gflops << " GFLOPS" << std::endl;
    std::cout << "Execution time from PAPI timer function: " << elapsed_time_sec << std::endl;

    return T.elapsedString();
}
#endif

int main(){
    auto const number_of_runs = 100;
    std::cout << "N\t\tElapsed." << std::endl;
    for (int N = 8; N < 1024;  N *= 2){
        auto elapsed = benchmark(N, N, N, number_of_runs);
        std::cout << N << "\t\t" << elapsed << std::endl;
    }
    return 0;
}