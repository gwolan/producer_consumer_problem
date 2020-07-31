#include <ProducerConsumer/Buffer.hpp>


Buffer::Buffer(uint32_t bufferCapacity, Graphics& graphics)
    : _bufferCapacity(bufferCapacity)
    , _bufferAllocation(0)
    , _graphics(graphics)
{ }

Buffer::~Buffer()
{ }

bool Buffer::putElementIntoBuffer(const std::string& producerName)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!isBufferFull())
    {
        _bufferAllocation++;
        _graphics.handlePutOperation(producerName, "puts value into buffer");
        return true;
    }

    _graphics.pushNewEventToLog(producerName, "buffer is full. Value not delivered");
    return false;
}

bool Buffer::getElementFromBuffer(const std::string& consumerName)
{
    std::lock_guard<std::mutex> lock(mutex);

    if(!isBufferEmpty())
    {
        _bufferAllocation--;
        _graphics.handleGetOperation(consumerName, "gets value from buffer");
        return true;
    }

    _graphics.pushNewEventToLog(consumerName, "buffer is empty. Value not taken");
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
