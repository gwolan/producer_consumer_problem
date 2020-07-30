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
    Graphics(uint32_t bufferCapacity);
    ~Graphics();

    void display();
    void pushNewEventToLog(const std::string& event);
    void incrementBufferAllocation();
    void decrementBufferAllocation();


    private:
    void shiftAndExtendMenuItems();
    uint32_t calculateWindowsWidth();
    uint32_t calculateBufferHeight();
    uint32_t calculateLogHeight();
    void calculateBufferWindowAllocationOffsets();
    void init();
    void refreshMenu();
    void raiseAllocationLevel();
    void decreaseAllocationLevel();
    void updateBufferStatus();

    WINDOW* _bufferWindow;
    WINDOW* _logWindow;
    MENU* _menu;
    ITEM** _menuItems;

    std::vector<std::string> _events;
    const std::string _nullRow;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPaddingForBufferAndLog;
    const uint32_t _spaceSizeBetweenBufferAndLog;
    uint32_t _windowsWidth;

    uint32_t _bufferWindowHeight;
    uint32_t _bufferElementsAmount;
    uint32_t _bufferMinElementsRequiredToDrawLine;
    uint32_t _bufferMaxElementsRequiredToDrawLine;
    uint32_t _bufferMaxElementsAlreadyDrawn;
    uint32_t _bufferMinElementsAlreadyDrawn;
    uint32_t _bufferAllowedMaxElementsToDraw;
    uint32_t _bufferAllowedMinElementsToDraw;
    uint32_t _bufferCurrentRowIndex;
    const uint32_t _bufferCapacity;

    uint32_t _menuHeight;
    uint32_t _menuWidth;
    uint32_t _logHeight;
    uint32_t _logRecords;
    const uint32_t _logsCapacity;

    Logger _logger;
};
