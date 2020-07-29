#pragma once

#include <cstdint>


class IO
{
    public:
    IO();
    ~IO() = default;

    bool validateArgs(uint32_t argsCount, char** argv);
    uint32_t getInputArgValue();


    private:
    bool readInputArg(const char* arg);

    uint32_t _inputArgValue;
};
