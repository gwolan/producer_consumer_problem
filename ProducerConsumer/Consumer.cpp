#include <ProducerConsumer/Consumer.hpp>


Consumer::Consumer(Buffer& buffer, const std::string& name, StartingLine& startingLine)
    : _buffer(buffer)
    , _startingLine(startingLine)
    , _consumer(&Consumer::startConsuming, this)
    , _consume(true)
    , _stoppedPromise()
    , _timerDice(1500, 3000)
    , _consumerName(name)
{ }

Consumer::~Consumer()
{
    if(_consumer.joinable())
    {
        _consumer.detach();
    }

    stop();
}

void Consumer::startConsuming()
{
    _startingLine.wait();

    while(_consume)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_timerDice.rollUnsignedInt()));
        _buffer.getElementFromBuffer(_consumerName);
    }

    _stoppedPromise.set_value(true);
}

void Consumer::stop()
{
    _consume = false;
}

std::future<bool> Consumer::getFutureStopped()
{
    return _stoppedPromise.get_future();
}
