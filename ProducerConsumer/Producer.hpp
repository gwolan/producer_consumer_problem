#pragma once

#include <thread>
#include <string>
#include <future>
#include <ProducerConsumer/StartingLine.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Producer
{
    public:
    Producer(Buffer& buffer, const std::string& name, StartingLine& startingLine);
    ~Producer();

    void stop();
    std::future<bool> getFutureStopped();


    private:
    void startProduction();

    Buffer& _buffer;
    StartingLine& _startingLine;
    std::thread _producer;
    bool _produce;
    std::promise<bool> _stoppedPromise;
    Dice _timerDice;
    std::string _producerName;
};
