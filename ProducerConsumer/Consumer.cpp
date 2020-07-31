#include <ProducerConsumer/Consumer.hpp>


Consumer::Consumer(Buffer& buffer, const std::string& name, StartingLine& startingLine)
    : _buffer(buffer)
    , _startingLine(startingLine)
    , _consumer(&Consumer::startConsuming, this)
    , _consume(true)
    , _timerDice(1500, 3000)
    , _consumerName(name)
{ }

Consumer::~Consumer()
{
    stop();
    if(_consumer.joinable())
    {
        _consumer.detach();
    }
}

void Consumer::startConsuming()
{
    _startingLine.wait();

    while(_consume)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_timerDice.rollUnsignedInt()));
        _buffer.getElementFromBuffer(_consumerName);
    }
}

void Consumer::stop()
{
    _consume = false;
}
