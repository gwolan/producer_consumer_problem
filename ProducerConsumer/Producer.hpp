#pragma once

#include <thread>
#include <string>
#include <ProducerConsumer/StartingLine.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Producer
{
    public:
    Producer(Buffer& buffer, const std::string& name, StartingLine& startingLine);
    ~Producer();

    void start();
    void stop();


    private:
    void startProduction();

    Buffer& _buffer;
    StartingLine& _startingLine;
    std::thread _producer;
    bool _produce;
    Dice _timerDice;
    std::string _producerName;
};
