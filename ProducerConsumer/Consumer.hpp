#pragma once

#include <thread>
#include <string>
#include <future>
#include <ProducerConsumer/StartingLine.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Consumer
{
    public:
    Consumer(Buffer& buffer, const std::string& name, StartingLine& startingLine);
    ~Consumer();

    void stop();
    std::future<bool> getFutureStopped();


    private:
    void startConsuming();

    Buffer& _buffer;
    StartingLine& _startingLine;
    std::thread _consumer;
    bool _consume;
    std::promise<bool> _stoppedPromise;
    Dice _timerDice;
    std::string _consumerName;
};
