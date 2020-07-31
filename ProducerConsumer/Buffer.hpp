#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <condition_variable>
#include <Console/Graphics.hpp>


class Buffer
{
    public:
    Buffer(uint32_t bufferCapacity, Graphics& graphics);
    ~Buffer() = default;

    bool putElementIntoBuffer(const std::string& producerName);
    bool getElementFromBuffer(const std::string& consumerName);


    private:
    bool isBufferFull();
    bool isBufferEmpty();

    const uint32_t _bufferCapacity;
    uint32_t _bufferAllocation;
    Graphics& _graphics;
    std::mutex _mutex;
    std::condition_variable _producerStopper;
    std::condition_variable _consumerStopper;
};
