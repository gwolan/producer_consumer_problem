#include <ProducerConsumer/ProducerConsumer.hpp>


ProducerConsumer::ProducerConsumer(uint32_t bufferCapacity)
    : _bufferCapacity(bufferCapacity)
    , _defaultProducersCount(5)
    , _defaultConsumersCount(5)
    , _graphics(_bufferCapacity)
    , _buffer(_bufferCapacity, _graphics)
{ }

ProducerConsumer::~ProducerConsumer()
{ }

void ProducerConsumer::createProducer(uint32_t producerId)
{
    _producers.emplace_back(_buffer, "Producer" + std::to_string(producerId));
}

void ProducerConsumer::createConsumer(uint32_t consumerId)
{
    _consumers.emplace_back(_buffer, "Consumer" + std::to_string(consumerId));
}

void ProducerConsumer::createActors()
{
    bool keepCreating = (_producers.size() < _defaultProducersCount) || (_consumers.size() < _defaultConsumersCount);
    bool createProducerFirst = true;

    while(keepCreating)
    {
        if(createProducerFirst)
        {
            if(_producers.size() < _defaultProducersCount)
            {
                createProducer(_producers.size());
            }

            if(_consumers.size() < _defaultConsumersCount)
            {
                createConsumer(_consumers.size());
            }

            createProducerFirst = false;
        }
        else
        {
            if(_consumers.size() < _defaultConsumersCount)
            {
                createConsumer(_consumers.size());
            }

            if(_producers.size() < _defaultProducersCount)
            {
                createProducer(_producers.size());
            }

            createProducerFirst = true;
        }

        keepCreating = (_producers.size() < _defaultProducersCount) || (_consumers.size() < _defaultConsumersCount);
    }
}

void ProducerConsumer::run()
{
    createActors();

    int32_t option;
    bool quit = false;
    while(!quit && (option = getch()))
    {
        switch(option)
        {
            case 'q':
                quit = true;
                break;
            default:
                _graphics.handleUserInput(option);
        }
    }
}
