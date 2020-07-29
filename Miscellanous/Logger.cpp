#include <Miscellanous/Logger.hpp>


// prologue can be a class name for example or empty string
Logger::Logger(const std::string& prologue)
    : prefix(prologue)
    , logFileName("logs.txt")
    , logFile()
{
    logFile.open(logFileName, std::ofstream::app);
}

Logger::~Logger()
{
    logFile.close();
}

void Logger::log(const std::string& log, const std::string functionName)
{
    if(logFile.is_open())
    {
        logFile << prefix << functionName << ": " << log << "\n";
        logFile.flush();
    }
}