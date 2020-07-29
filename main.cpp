#include <string>
#include <vector>
#include <iostream>
#include <Console/IO.hpp>


int main(int argc, char* argv[])
{
    IO io;

    // input argument define number of rows
    if(io.validateArgs((uint32_t)argc, argv))
    {

    }
    else
    {
        std::cout << "Invalid arguments!" << std::endl;
        std::cout << "Program accepts unsigned integer as number of required philosophers." << std::endl;
    }

    return 0;
}
