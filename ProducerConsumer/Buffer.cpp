#include <ProducerConsumer/Buffer.hpp>


Buffer::Buffer(uint32_t bufferCapacity, Graphics& graphics)
    : _bufferCapacity(bufferCapacity)
    , _bufferAllocation(0)
    , _graphics(graphics)
{ }

bool Buffer::putElementIntoBuffer(const std::string& producerName)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if(!isBufferFull())
    {
        _bufferAllocation++;
        _graphics.handlePutOperation(producerName, "puts value into buffer");
        _consumerStopper.notify_all();
        return true;
    }
    else
    {
        _graphics.pushNewEventToLog(producerName, "buffer is full. Value not delivered");
        _producerStopper.wait(lock, [this]{ return !this->isBufferFull(); });
        return false;
    }

}

bool Buffer::getElementFromBuffer(const std::string& consumerName)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if(!isBufferEmpty())
    {
        _bufferAllocation--;
        _graphics.handleGetOperation(consumerName, "gets value from buffer");
        _producerStopper.notify_all();
        return true;
    }
    else
    {
        _graphics.pushNewEventToLog(consumerName, "buffer is empty. Value not taken");
        _consumerStopper.wait(lock, [this]{ return !this->isBufferEmpty(); });
        return false;
    }

    return false;
}

bool Buffer::isBufferFull()
{
    return _bufferAllocation == _bufferCapacity;
}

bool Buffer::isBufferEmpty()
{
    return _bufferAllocation == 0;
}
