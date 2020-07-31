#pragma once

#include <thread>
#include <string>
#include <ProducerConsumer/StartingLine.hpp>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Consumer
{
    public:
    Consumer(Buffer& buffer, const std::string& name, StartingLine& startingLine);
    ~Consumer();

    void stop();


    private:
    void startConsuming();

    Buffer& _buffer;
    StartingLine& _startingLine;
    std::thread _consumer;
    bool _consume;
    Dice _timerDice;
    std::string _consumerName;
};
