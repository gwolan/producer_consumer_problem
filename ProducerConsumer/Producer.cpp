#include <ProducerConsumer/Producer.hpp>


Producer::Producer(Buffer& buffer, const std::string& name, StartingLine& startingLine)
    : _buffer(buffer)
    , _startingLine(startingLine)
    , _producer(&Producer::startProduction, this)
    , _produce(true)
    , _stoppedPromise()
    , _timerDice(1500, 3000)
    , _producerName(name)
{ }

Producer::~Producer()
{
    if(_producer.joinable())
    {
        _producer.detach();
    }

    stop();
}

void Producer::startProduction()
{
    _startingLine.wait();

    while(_produce)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_timerDice.rollUnsignedInt()));
        _buffer.putElementIntoBuffer(_producerName);
    }

    _stoppedPromise.set_value(true);
}

void Producer::stop()
{
    _produce = false;
}

std::future<bool> Producer::getFutureStopped()
{
    return _stoppedPromise.get_future();
}
