#pragma once

#include <fstream>
#include <string>


class Logger
{
    public:
    Logger(const std::string& prologue);
    ~Logger();

    void log(const std::string& log, const std::string functionName);


    private:
    const std::string prefix;
    const std::string logFileName;
    std::ofstream logFile;
};
