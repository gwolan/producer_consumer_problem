#include <ProducerConsumer/Producer.hpp>


Producer::Producer(Buffer& buffer, const std::string& name, StartingLine& startingLine)
    : _buffer(buffer)
    , _startingLine(startingLine)
    , _producer(&Producer::startProduction, this)
    , _produce(true)
    , _timerDice(1500, 3000)
    , _producerName(name)
{ }

Producer::~Producer()
{
    stop();
    if(_producer.joinable())
    {
        _producer.detach();
    }
}

void Producer::startProduction()
{
    _startingLine.wait();

    while(_produce)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_timerDice.rollUnsignedInt()));
        _buffer.putElementIntoBuffer(_producerName);
    }
}

void Producer::stop()
{
    _produce = false;
}
