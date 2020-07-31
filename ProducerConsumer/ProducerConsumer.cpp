#include <ProducerConsumer/ProducerConsumer.hpp>


ProducerConsumer::ProducerConsumer(uint32_t bufferCapacity)
    : _bufferCapacity(bufferCapacity)
    , _defaultProducersCount(5)
    , _defaultConsumersCount(5)
    , _graphics(_bufferCapacity, _defaultProducersCount, _defaultConsumersCount)
    , _buffer(_bufferCapacity, _graphics)
{ }

void ProducerConsumer::createProducer(uint32_t producerId)
{
    _producers.emplace_back(_buffer, "Producer" + std::to_string(producerId), _startingLine);
}

void ProducerConsumer::createConsumer(uint32_t consumerId)
{
    _consumers.emplace_back(_buffer, "Consumer" + std::to_string(consumerId), _startingLine);
}

void ProducerConsumer::destroyProducer()
{
    _producers.rbegin()->stop();
    if(_producers.rbegin()->getFutureStopped().get())
    {
        _producers.pop_back();
    }
}

void ProducerConsumer::destroyConsumer()
{
    _consumers.rbegin()->stop();
    if(_consumers.rbegin()->getFutureStopped().get())
    {
        _consumers.pop_back();
    }
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
                _graphics.updateProducersNumber(_producers.size());
                break;
            case 'z':
                destroyProducer();
                _graphics.updateProducersNumber(_producers.size());
                break;
            case 's':
                createConsumer(_consumers.size());
                _startingLine.start();
                _graphics.updateConsumersNumber(_consumers.size());
                break;
            case 'x':
                destroyConsumer();
                _graphics.updateConsumersNumber(_consumers.size());
                break;
            case 'q':
                quit = true;
                break;
            default:
                _graphics.handleUserInput(option);
                break;
        }
    }
}
