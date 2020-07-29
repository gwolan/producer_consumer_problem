#include <utility>
#include <Console/Graphics.hpp>


Graphics::Graphics(uint32_t bufferCapacity)
    : _bufferWindow(nullptr)
    , _logWindow(nullptr)
    , _menu(nullptr)
    , _menuItems(new ITEM*[100])
    , _events()
    , _nullRow("\0")
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPadding(50)
    , _spaceSizeBetweenBufferAndLog(3)
    , _bufferCapacity(bufferCapacity)
    , _bufferAllocationOffset(0)
    , _bufferAllocation(0)
    , _rowsCapacity(100)
    , _rowsCount(0)
    , _logger("Console")
{
    // set COLS and LINES variables
    initscr();

    init();
    pushNewEventToLog(_nullRow);
}

Graphics::~Graphics()
{
    wrefresh(_bufferWindow);
    wrefresh(_logWindow);

    // deallocate menu
    unpost_menu(_menu);
    free_menu(_menu);

    // deallocate menu items
    for(uint32_t rowId = 0; rowId < _rowsCount; ++rowId)
    {
        free_item(_menuItems[rowId]);
    }

    // deallocate window
    wborder(_logWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wborder(_bufferWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    refresh();
    delwin(_logWindow);
    delwin(_bufferWindow);

    // stop ncurses
    endwin();
}

void Graphics::shiftAndExtendMenuItems()
{
    if(_rowsCount == _rowsCapacity)
    {
        ITEM** newMenuItems = new ITEM*[2*_rowsCapacity];

        for(uint32_t rowId = 0; rowId < _rowsCount; ++rowId)
        {
            newMenuItems[rowId + 1] = _menuItems[rowId];
        }

        _menuItems = newMenuItems;
    }
    else
    {
        for(uint32_t rowId = _rowsCount; rowId > 0; --rowId)
        {
            _menuItems[rowId] = _menuItems[rowId - 1];
        }
    }
}

uint32_t Graphics::calculateWindowsWidth()
{
    return COLS - 2*_horizontalPadding;
}

uint32_t Graphics::calculateBufferHeight()
{
    return ((LINES - _topPadding - _bottomPadding) * 0.8) - _spaceSizeBetweenBufferAndLog;
}

uint32_t Graphics::calculateLogHeight()
{
    return (LINES - _topPadding - _bottomPadding) * 0.2;
}

void Graphics::init()
{
    // block OS signal receiver for special characters combinations like Ctrl + C
    raw();

    // do not print user selections on the screen
    noecho();

    // give keypad control to the main screen
    keypad(stdscr, true);

    uint32_t bufferHeight = calculateBufferHeight();
    uint32_t logHeight = calculateLogHeight();
    uint32_t windowsWidth = calculateWindowsWidth();
    uint32_t menuHeight = logHeight - 2;
    uint32_t menuWidth = windowsWidth - 2;
    _bufferAllocationOffset = bufferHeight / _bufferCapacity;

    // allocate memory for parent window and its sibling menu
    _menu = new_menu(_menuItems);
    _bufferWindow = newwin(bufferHeight, windowsWidth, _topPadding, _horizontalPadding);
    _logWindow = newwin(logHeight, windowsWidth, _topPadding + bufferHeight + _spaceSizeBetweenBufferAndLog, _horizontalPadding);
    keypad(_logWindow, true);

    // associate menu with window
    set_menu_win(_menu, _logWindow);

    // create sub window for menu
    set_menu_sub(_menu, derwin(_logWindow, menuHeight, menuWidth, 1, 1));

    // create sub window for menu
    set_menu_format(_menu, menuHeight, 1);

    // hide marking char for current menu selection
    set_menu_mark(_menu, "");

    // draw simple box sround window
    box(_logWindow, 0, 0);
    box(_bufferWindow, 0, 0);

    // print heading and user tips
    mvprintw(0, _horizontalPadding, "%s", "Author: Wolanski Grzegorz");
    mvprintw(_topPadding - 1, _horizontalPadding + 1, "%s - %d/%d", "Buffer", _bufferAllocation, _bufferCapacity);
    mvprintw(_topPadding + bufferHeight + _spaceSizeBetweenBufferAndLog - 1, _horizontalPadding + 1, "%s", "Actions log");
    mvprintw(_topPadding + bufferHeight + logHeight + 2, _horizontalPadding, "%s", "Q - exit program");
    refresh();

    // generate menu with items in the memory of the window
    post_menu(_menu);
    wrefresh(_bufferWindow);
    wrefresh(_logWindow);
    refresh();
}

void Graphics::refreshMenu()
{
    unpost_menu(_menu);
    set_menu_items(_menu, _menuItems);
    post_menu(_menu);
    wrefresh(_logWindow);
}

void Graphics::display()
{
    int32_t option;
    uint32_t id = 0;
    bool quit = false;

    while(!quit && (option = getch()))
    {
        std::lock_guard<std::mutex> lock(mutex);

        switch(option)
        {
            case KEY_DOWN:
                // choose lower item
                menu_driver(_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                // choose higher item
                menu_driver(_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                // scroll to next page
                menu_driver(_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                // scroll to previous page
                menu_driver(_menu, REQ_SCR_UPAGE);
                break;
            case 'a':
                pushNewEventToLog("Producer" + std::to_string(id++) + " added resource to the buffer");
                break;
            case 's':
                pushNewEventToLog("Consumer" + std::to_string(id++) + " took resource from the buffer");
                break;
            case 'd':
                pushNewEventToLog("Producer" + std::to_string(id++) + " stopped. Buffer is full");
                break;
            case 'f':
                pushNewEventToLog("Consumer" + std::to_string(id++) + " stopped. Buffer is empty");
                break;
            case 'q':
                quit = true;
                break;
            default:
                break;
        }

        wrefresh(_logWindow);
    }
}

void Graphics::pushNewEventToLog(const std::string& event)
{
    _logger.log(event, "");
    //std::lock_guard<std::mutex> lock(mutex);

    shiftAndExtendMenuItems();
    _events.insert(_events.begin(), event);
    _menuItems[0] = new_item(_events.front().c_str(), "");
    _rowsCount++;

    refreshMenu();
}

void Graphics::incrementBufferAllocation()
{

}
