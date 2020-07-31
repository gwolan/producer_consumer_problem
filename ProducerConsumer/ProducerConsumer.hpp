#pragma once

#include <ncurses.h>
#include <cstdint>
#include <list>
#include <Console/Graphics.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <ProducerConsumer/Producer.hpp>
#include <ProducerConsumer/Consumer.hpp>
#include <ProducerConsumer/StartingLine.hpp>


class ProducerConsumer
{
    public:
    ProducerConsumer(uint32_t bufferCapacity);
    ~ProducerConsumer() = default;

    void run();


    private:
    void createActors();
    void createProducer(uint32_t producerId);
    void createConsumer(uint32_t consumerId);
    void destroyProducer();
    void destroyConsumer();

    const uint32_t _bufferCapacity;
    const uint32_t _defaultProducersCount;
    const uint32_t _defaultConsumersCount;
    Graphics _graphics;
    Buffer _buffer;
    std::list<Producer> _producers;
    std::list<Consumer> _consumers;
    StartingLine _startingLine;
};
