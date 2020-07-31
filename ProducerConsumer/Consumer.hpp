#pragma once

#include <thread>
#include <string>
#include <ProducerConsumer/Buffer.hpp>
#include <Miscellanous/Dice.hpp>


class Consumer
{
    public:
    Consumer(Buffer& buffer, const std::string& name);
    ~Consumer();

    void stop();


    private:
    void startConsuming();

    Buffer& _buffer;
    std::thread _consumer;
    bool _consume;
    Dice _timerDice;
    std::string _consumerName;
};
