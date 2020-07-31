#include <utility>
#include <Console/Graphics.hpp>


Graphics::Graphics(uint32_t bufferCapacity)
    : _bufferWindow(nullptr)
    , _logWindow(nullptr)
    , _menu(nullptr)
    , _menuItems(new ITEM*[20])
    , _events()
    , _nullRow("\0")
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPaddingForBufferAndLog(50)
    , _spaceSizeBetweenBufferAndLog(3)
    , _bufferElementsAmount(0)
    , _bufferMaxElementsAlreadyDrawn(0)
    , _bufferMinElementsAlreadyDrawn(0)
    , _bufferAddedElementsCounter(0)
    , _bufferCapacity(bufferCapacity)
    , _logRecords(0)
    , _logsCapacity(20)
    , _logger("Console")
{
    // set COLS and LINES variables
    initscr();

    // initialize rest of the variables that are dependent on main window size
    _bufferWindowHeight = calculateBufferHeight();
    _logHeight = calculateLogHeight();
    _windowsWidth = calculateWindowsWidth();
    _menuHeight = _logHeight - 2;
    _menuWidth = _windowsWidth - 2;
    _bufferCurrentRowIndex = _bufferWindowHeight - 1;
    calculateBufferWindowAllocationOffsets();

    _events.push_back(_nullRow);
    _menuItems[0] = new_item(_events.front().c_str(), "");
    init();
}

Graphics::~Graphics()
{
    wrefresh(_bufferWindow);
    wrefresh(_logWindow);

    // deallocate menu
    unpost_menu(_menu);
    free_menu(_menu);

    // deallocate menu items
    for(uint32_t logId = 0; logId < _logRecords; ++logId)
    {
        free_item(_menuItems[logId]);
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
    if(_logRecords == _logsCapacity)
    {
        free_item(_menuItems[_logRecords - 1]);
        for(uint32_t logId = _logRecords - 1; logId > 0; --logId)
        {
            _menuItems[logId] = _menuItems[logId - 1];
        }
    }
    else
    {
        for(uint32_t logId = _logRecords; logId > 0; --logId)
        {
            _menuItems[logId] = _menuItems[logId - 1];
        }
    }
}

void Graphics::calculateBufferWindowAllocationOffsets()
{
    uint32_t bufferWindowAvailableSpace = _bufferWindowHeight - 2;
    _bufferMinElementsRequiredToDrawLine = _bufferCapacity / bufferWindowAvailableSpace;
    _bufferAllowedMaxElementsToDraw = _bufferCapacity % bufferWindowAvailableSpace;
    _bufferAllowedMinElementsToDraw = bufferWindowAvailableSpace - _bufferAllowedMaxElementsToDraw;

    if(_bufferAllowedMaxElementsToDraw == 0)
    {
        _bufferMaxElementsRequiredToDrawLine = _bufferMinElementsRequiredToDrawLine;
    }
    else
    {
        _bufferMaxElementsRequiredToDrawLine = _bufferMinElementsRequiredToDrawLine + 1;
    }
}

uint32_t Graphics::calculateWindowsWidth()
{
    return COLS - 2*_horizontalPaddingForBufferAndLog;
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

    // allocate memory for parent window and its sibling menu
    _menu = new_menu(_menuItems);
    _bufferWindow = newwin(_bufferWindowHeight, _windowsWidth, _topPadding, _horizontalPaddingForBufferAndLog);
    _logWindow = newwin(_logHeight, _windowsWidth, _topPadding + _bufferWindowHeight + _spaceSizeBetweenBufferAndLog, _horizontalPaddingForBufferAndLog);
    keypad(_logWindow, true);

    // associate menu with window
    set_menu_win(_menu, _logWindow);

    // create sub window for menu
    set_menu_sub(_menu, derwin(_logWindow, _menuHeight, _menuWidth, 1, 1));

    // create sub window for menu
    set_menu_format(_menu, _menuHeight, 1);

    // hide marking char for current menu selection
    set_menu_mark(_menu, "");

    // draw simple box sround window
    box(_logWindow, 0, 0);
    box(_bufferWindow, 0, 0);

    // print heading and user tips
    mvprintw(0, _horizontalPaddingForBufferAndLog, "%s", "Author: Wolanski Grzegorz");
    mvprintw(_topPadding - 1, _horizontalPaddingForBufferAndLog + 1, "%s - %d/%d", "Buffer", _bufferElementsAmount, _bufferCapacity);
    mvprintw(_topPadding + _bufferWindowHeight + _spaceSizeBetweenBufferAndLog - 1, _horizontalPaddingForBufferAndLog + 1, "%s", "Actions log");
    mvprintw(_topPadding + _bufferWindowHeight + _logHeight + 2, _horizontalPaddingForBufferAndLog, "%s", "Q - exit program");
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

void Graphics::handleUserInput(int32_t option)
{
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
        default:
            break;
    }

    wrefresh(_logWindow);
}

void Graphics::updateBufferStatus()
{
    mvprintw(_topPadding - 1, _horizontalPaddingForBufferAndLog + 1, "%s - %d/%d                               ", "Buffer", _bufferElementsAmount, _bufferCapacity);
    refresh();
}

void Graphics::pushNewEventToLog(const std::string& actor, const std::string& event)
{
    std::string wholeEvent(actor + " - " + event);
    _logger.log(wholeEvent, "");

    shiftAndExtendMenuItems();
    _events.insert(_events.begin(), wholeEvent);
    _menuItems[0] = new_item(_events.front().c_str(), "");
    _logRecords++;

    refreshMenu();
}

void Graphics::incrementAddedElementsCounter(uint32_t upperBound)
{
    if(_bufferAddedElementsCounter >= upperBound)
    {
        _bufferAddedElementsCounter = 1;
    }
    else
    {
        _bufferAddedElementsCounter++;
    }
}

void Graphics::decrementAddedElementsCounter(uint32_t upperBound)
{
    if(_bufferAddedElementsCounter == 0)
    {
        _bufferAddedElementsCounter = upperBound - 1;
    }
    else
    {
        _bufferAddedElementsCounter--;
    }
}

void Graphics::raiseAllocationLevel()
{
    wattron(_bufferWindow, A_REVERSE);
    mvwhline(_bufferWindow, --_bufferCurrentRowIndex, 1, ' ', _windowsWidth - 2);
    wattroff(_bufferWindow, A_REVERSE);

    wrefresh(_bufferWindow);
}

void Graphics::decreaseAllocationLevel()
{
    mvwhline(_bufferWindow, _bufferCurrentRowIndex++, 1, ' ', _windowsWidth - 2);

    wrefresh(_bufferWindow);
}

void Graphics::incrementBufferAllocation()
{
    _bufferElementsAmount++;
    updateBufferStatus();

    if(_bufferMaxElementsAlreadyDrawn < _bufferAllowedMaxElementsToDraw)
    {
        incrementAddedElementsCounter(_bufferMaxElementsRequiredToDrawLine);
        if(_bufferAddedElementsCounter == _bufferMaxElementsRequiredToDrawLine)
        {
            _bufferMaxElementsAlreadyDrawn++;
            raiseAllocationLevel();
        }
    }
    else
    {
        incrementAddedElementsCounter(_bufferMinElementsRequiredToDrawLine);
        if(_bufferAddedElementsCounter == _bufferMinElementsRequiredToDrawLine)
        {
            _bufferMinElementsAlreadyDrawn++;
            raiseAllocationLevel();
        }
    }
}

void Graphics::decrementBufferAllocation()
{
    _bufferElementsAmount--;
    updateBufferStatus();

    if(_bufferMinElementsAlreadyDrawn != 0)
    {
        decrementAddedElementsCounter(_bufferMinElementsRequiredToDrawLine);
        if(_bufferAddedElementsCounter == (_bufferMinElementsRequiredToDrawLine - 1))
        {
            _bufferMinElementsAlreadyDrawn--;
            decreaseAllocationLevel();
        }
    }
    else
    {
        decrementAddedElementsCounter(_bufferMaxElementsRequiredToDrawLine);
        if(_bufferAddedElementsCounter == (_bufferMaxElementsRequiredToDrawLine - 1))
        {
            _bufferMaxElementsAlreadyDrawn--;
            decreaseAllocationLevel();
        }
    }
}

void Graphics::handlePutOperation(const std::string& actor, const std::string& event)
{
    pushNewEventToLog(actor, event);
    incrementBufferAllocation();
}

void Graphics::handleGetOperation(const std::string& actor, const std::string& event)
{
    pushNewEventToLog(actor, event);
    decrementBufferAllocation();
}
