#include <ProducerConsumer/ProducerConsumer.hpp>


ProducerConsumer::ProducerConsumer(uint32_t bufferCapacity)
    : _bufferCapacity(bufferCapacity)
    , _defaultProducersCount(5)
    , _defaultConsumersCount(5)
    , _graphics(_bufferCapacity)
    , _buffer(_bufferCapacity, _graphics)
{ }

void ProducerConsumer::createProducer(uint32_t producerId)
{
    _producers.emplace_back(_buffer, "Producer" + std::to_string(producerId), _startingLine);
    //_graphics.pushNewEventToLog("Producer" + std::to_string(producerId), "was created");
}

void ProducerConsumer::createConsumer(uint32_t consumerId)
{
    _consumers.emplace_back(_buffer, "Consumer" + std::to_string(consumerId), _startingLine);
    //_graphics.pushNewEventToLog("Consumer" + std::to_string(consumerId), "was created");
}

void ProducerConsumer::destroyProducer()
{
    _producers.rbegin()->stop();
    if(_producers.rbegin()->getFutureStopped().get())
    {
        _producers.pop_back();
    }
    //_graphics.pushNewEventToLog("Producer" + std::to_string(_producers.size()), "was destroyed");
}

void ProducerConsumer::destroyConsumer()
{
    _consumers.rbegin()->stop();
    if(_consumers.rbegin()->getFutureStopped().get())
    {
        _consumers.pop_back();
    }
    //_graphics.pushNewEventToLog("Consumer" + std::to_string(_consumers.size()), "was destroyed");
}

void ProducerConsumer::createActors()
{
    for(uint32_t actorId = 0; actorId < _defaultProducersCount; ++actorId)
    {
        createProducer(actorId);
    }

    for(uint32_t actorId = 0; actorId < _defaultConsumersCount; ++actorId)
    {
        createConsumer(actorId);
    }
}

void ProducerConsumer::run()
{
    createActors();
    _startingLine.start();

    int32_t option;
    bool quit = false;
    while(!quit && (option = getch()))
    {
        switch(option)
        {
            case 'a':
                createProducer(_producers.size());
                _startingLine.start();
                break;
            case 'z':
                destroyProducer();
                break;
            case 's':
                createConsumer(_consumers.size());
                _startingLine.start();
                break;
            case 'x':
                destroyConsumer();
                break;
            case 'q':
                quit = true;
                break;
            default:
                _graphics.handleUserInput(option);
        }
    }
}
