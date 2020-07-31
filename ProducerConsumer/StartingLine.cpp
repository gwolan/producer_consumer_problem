#include<ProducerConsumer/StartingLine.hpp>


void StartingLine::wait()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _startingLine.wait(lock);
}

void StartingLine::start()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _startingLine.notify_all();
}
