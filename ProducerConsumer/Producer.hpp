#pragma once

#include <thread>
#include <string>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Producer
{
    public:
    Producer(Buffer& buffer, const std::string& name);
    ~Producer();

    void stop();


    private:
    void startProduction();

    Buffer& _buffer;
    std::thread _producer;
    bool _produce;
    Dice _timerDice;
    std::string _producerName;
};
