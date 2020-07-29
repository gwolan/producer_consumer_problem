#pragma once

#include <menu.h>
#include <ncurses.h>
#include <cstdint>
#include <vector>
#include <mutex>
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


    private:
    void shiftAndExtendMenuItems();
    uint32_t calculateWindowsWidth();
    uint32_t calculateBufferHeight();
    uint32_t calculateLogHeight();
    void init();
    void refreshMenu();

    WINDOW* _bufferWindow;
    WINDOW* _logWindow;
    MENU* _menu;
    ITEM** _menuItems;

    std::mutex mutex;
    std::vector<std::string> _events;
    const std::string _nullRow;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPadding;
    const uint32_t _spaceSizeBetweenBufferAndLog;
    const uint32_t _bufferCapacity;
    uint32_t _bufferAllocationOffset;
    uint32_t _bufferAllocation;
    uint32_t _rowsCapacity;
    uint32_t _rowsCount;

    Logger _logger;
};
