#pragma once

#include <chrono>


class Timer
{
    public:
    Timer() = default;
    ~Timer() = default;

    void start();
    void stop();
    double getTime();


    private:
    std::chrono::time_point<std::chrono::system_clock> begin;
    std::chrono::time_point<std::chrono::system_clock> end;
};
