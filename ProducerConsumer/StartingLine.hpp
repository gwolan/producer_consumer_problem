#pragma once

#include<mutex>
#include<condition_variable>


class StartingLine
{
    public:
    StartingLine() = default;
    ~StartingLine() = default;

    void start();
    void wait();

    private:
    std::mutex _mutex;
    std::condition_variable _startingLine;
};
