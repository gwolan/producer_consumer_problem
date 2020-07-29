#include <string>
#include <Console/IO.hpp>


IO::IO()
    : _inputArgValue(5)
{ }

bool IO::validateArgs(uint32_t argsCount, char** argv)
{
    switch(argsCount)
    {
        case 1: return true;

        case 2: return readInputArg(argv[1]);

        default: break;
    }

    return false;
}

bool IO::readInputArg(const char* arg)
{
    try
    {
        uint32_t inputValue = std::stoi(arg);

        if(inputValue > 0)
        {
            _inputArgValue = inputValue;
            return true;
        }
    }
    catch(...) { }

    return false;
}

uint32_t IO::getInputArgValue()
{
    return _inputArgValue;
}
