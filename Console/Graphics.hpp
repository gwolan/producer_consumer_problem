#pragma once

#include <menu.h>
#include <ncurses.h>
#include <cstdint>
#include <vector>
#include <string>
#include <Miscellanous/Logger.hpp>

class Graphics
{
    public:
    Graphics(uint32_t bufferCapacity, uint32_t producersCount, uint32_t consumersCount);
    ~Graphics();

    void handleUserInput(int32_t option);
    void handlePutOperation(const std::string& actor, const std::string& event);
    void handleGetOperation(const std::string& actor, const std::string& event);
    void pushNewEventToLog(const std::string& actor, const std::string& event);
    void updateProducersNumber(uint32_t producersCount);
    void updateConsumersNumber(uint32_t consumersCount);


    private:
    void shiftAndExtendMenuItems();
    uint32_t calculateWindowsWidth();
    uint32_t calculateBufferHeight();
    uint32_t calculateLogHeight();
    void calculateBufferWindowAllocationOffsets();
    void init();
    void refreshMenu();
    void incrementAddedElementsCounter(uint32_t upperBound);
    void decrementAddedElementsCounter(uint32_t upperBound);
    void raiseAllocationLevel();
    void decreaseAllocationLevel();
    void updateBufferStatus();
    void incrementBufferAllocation();
    void decrementBufferAllocation();

    WINDOW* _bufferWindow;
    WINDOW* _logWindow;
    WINDOW* _producersWindow;
    WINDOW* _consumersWindow;
    MENU* _menu;
    ITEM** _menuItems;

    std::vector<std::string> _events;
    const std::string _nullRow;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPaddingForBufferAndLog;
    const uint32_t _spaceSizeBetweenBufferAndLog;
    uint32_t _windowsWidth;
    uint32_t _producersCount;
    uint32_t _consumersCount;

    uint32_t _bufferWindowHeight;
    uint32_t _bufferElementsAmount;
    uint32_t _bufferMinElementsRequiredToDrawLine;
    uint32_t _bufferMaxElementsRequiredToDrawLine;
    uint32_t _bufferMaxElementsAlreadyDrawn;
    uint32_t _bufferMinElementsAlreadyDrawn;
    uint32_t _bufferAllowedMaxElementsToDraw;
    uint32_t _bufferAllowedMinElementsToDraw;
    uint32_t _bufferAddedElementsCounter;
    uint32_t _bufferCurrentRowIndex;
    const uint32_t _bufferCapacity;

    uint32_t _menuHeight;
    uint32_t _menuWidth;
    uint32_t _logHeight;
    uint32_t _logRecords;
    const uint32_t _logsCapacity;

    Logger _logger;
};
