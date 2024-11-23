//
// Created by Bjørn Lindi on 17/11/2024.
//
#ifndef CPP4HPC_TIMER_H
#define CPP4HPC_TIMER_H
#include <chrono>

namespace cpp4hpc {
#include <chrono>
#include <iostream>
#include <string>

    class Timer {
    public:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        Timer() : start_time(Clock::now()), running(true) {}

        // Start the timer or restart it
        void start() {
            start_time = Clock::now();
            running = true;
        }

        // Stop the timer
        void stop() {
            if (running) {
                end_time = Clock::now();
                running = false;
            }
        }

        // Reset the timer
        void reset() {
            start_time = Clock::now();
            running = true;
        }

        // Get the elapsed time in a given unit
        template <typename Duration = std::chrono::milliseconds>
        auto elapsed() const {
            if (running) {
                return std::chrono::duration_cast<Duration>(Clock::now() - start_time).count();
            } else {
                return std::chrono::duration_cast<Duration>(end_time - start_time).count();
            }
        }

        // Helper function for elapsed time in a readable format
        std::string elapsedString() const {
            auto ms = elapsed<std::chrono::milliseconds>();
            auto s = ms / 1000;
            ms %= 1000;
            return std::to_string(s) + "s " + std::to_string(ms) + "ms";
        }

    private:
        TimePoint start_time;
        TimePoint end_time;
        bool running;
    };
}
#endif // CPP4HPC_TIMER_H