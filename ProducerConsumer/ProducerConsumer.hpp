#pragma once

#include <cstdint>
#include <list>
#include <ncurses.h>
#include <Console/Graphics.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <ProducerConsumer/Producer.hpp>
#include <ProducerConsumer/Consumer.hpp>


class ProducerConsumer
{
    public:
    ProducerConsumer(uint32_t bufferCapacity);
    ~ProducerConsumer();

    void run();


    private:
    void createActors();
    void createProducer(uint32_t producerId);
    void createConsumer(uint32_t consumerId);

    const uint32_t _bufferCapacity;
    const uint32_t _defaultProducersCount;
    const uint32_t _defaultConsumersCount;
    Graphics _graphics;
    Buffer _buffer;
    std::list<Producer> _producers;
    std::list<Consumer> _consumers;
};
