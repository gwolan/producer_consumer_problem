#include <string>
#include <vector>
#include <iostream>
#include <Console/IO.hpp>
#include <ProducerConsumer/ProducerConsumer.hpp>


int main(int argc, char* argv[])
{
    IO io;

    // input argument define number of rows
    if(io.validateArgs((uint32_t)argc, argv))
    {
        ProducerConsumer producerConsumer(io.getInputArgValue());
        producerConsumer.run();
    }
    else
    {
        std::cout << "Invalid arguments!" << std::endl;
        std::cout << "Program accepts unsigned integer as buffer capacity." << std::endl;
    }

    return 0;
}
