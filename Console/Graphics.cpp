#include <utility>
#include <Console/Graphics.hpp>


Graphics::Graphics(const std::vector<std::string>& columnsNames,
                   const std::vector<std::vector<std::string>>& rowsValues)
    : _window(nullptr)
    , _menu(nullptr)
    , _menuItems(nullptr)
    , _columnsNames(columnsNames)
    , _rows()
    , _rowsValues(rowsValues)
    , _nullRow("\0")
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPadding(4)
    , _columnsCount(columnsNames.size())
    , _columnWidth()
    , _rowsCount()
    , _baseMenuItemId(2)
    , _rowsForkStateIndex(4)
    , _rowsForkOwnerIndex(3)
{
    // set COLS and LINES variables
    initscr();

    // calculate each columns maximum string length
    _columnWidth = calculateColumnWidth();
    createRows();
    init();
}

Graphics::~Graphics()
{
    wrefresh(_window);

    // deallocate menu
    unpost_menu(_menu);
    free_menu(_menu);

    // deallocate menu items
    for(uint32_t rowId = 0; rowId < _rowsCount; ++rowId)
    {
        free_item(_menuItems[rowId]);
    }

    // deallocate window
    wborder(_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    refresh();
    delwin(_window);

    // stop ncurses
    endwin();
}

std::string Graphics::createRow(const std::vector<std::string>& columnsValues)
{
    // this method draws table row out of given values
    std::string result;
    uint32_t bordersCount = 0;

    for(const auto& columnValue : columnsValues)
    {
        // calculate padding between columns row content and column separator
        uint32_t padding = _columnWidth - columnValue.size();

        // build row
        result = result + columnValue + std::string(padding, ' ');

        if(++bordersCount != columnsValues.size())
        {
            // add column separator if not all columns were filled
            result = result + "|";
        }
        else
        {
            // append more padding to the last column to fill whole menu window size
            padding = calculateWindowWidth() - 2 - result.size();
            result = result + std::string(padding, ' ');
        }
    }

    return result;
}

void Graphics::createRows()
{
    // this method creates rows for table
    // first two rows are column titles and line separator
    // succesive rows are table records

    // create row containing column titles
    // create row containing line separator
    std::string menuTitle = createRow(_columnsNames);
    std::string lineSeparator = createRow(std::vector<std::string>(_columnsCount, std::string(_columnWidth, '-')));

    // push rows to the rows vector as table column names and line separator
    _rows.push_back(menuTitle);
    _rows.push_back(lineSeparator);

    // create following table records
    for(const auto& row : _rowsValues)
    {
        std::string menuItem = createRow(row);
        _rows.push_back(menuItem);
    }

    // ncurses menu requires that menu item list is closed with null item
    _rows.push_back(_nullRow);
    _rowsCount = _rows.size();

    // add columns names, line separator and null row to rows values vector
    _rowsValues.insert(_rowsValues.begin(), std::vector<std::string>(_columnsCount, std::string(_columnWidth, '-')));
    _rowsValues.insert(_rowsValues.begin(), _columnsNames);
    _rowsValues.push_back(std::vector<std::string>(1, _nullRow));
}

uint32_t Graphics::calculateWindowWidth()
{
    return COLS - 2*_horizontalPadding;
}

uint32_t Graphics::calculateWindowHeight()
{
    return LINES - _topPadding - _bottomPadding;
}

uint32_t Graphics::calculateColumnWidth()
{
    return ((calculateWindowWidth() - 1) - (_columnsCount - 1)) / _columnsCount;
}

void Graphics::initMenuItems()
{
    // this method creates actual menu items from created rows
    _menuItems = new ITEM*[_rows.size()];
    for(uint32_t rowId = 0; rowId < _rows.size() - 1; ++rowId)
    {
        _menuItems[rowId] = new_item(_rows[rowId].c_str(), _rows[rowId].c_str());
    }

    // add null item at the end
    _menuItems[_rows.size() - 1] = new_item(_rows.rbegin()->c_str(), _rows.rbegin()->c_str());
}

void Graphics::init()
{
    // block OS signal receiver for special characters combinations like Ctrl + C
    raw();

    // do not print user selections on the screen
    noecho();

    // give keypad control to the main screen
    keypad(stdscr, true);

    _columnWidth = calculateColumnWidth();
    uint32_t windowHeight = calculateWindowHeight();
    uint32_t windowWidth = calculateWindowWidth();
    uint32_t menuHeight = calculateWindowHeight() - 2;
    uint32_t menuWidth = calculateWindowWidth() - 2;

    initMenuItems();

    // allocate memory for parent window and its sibling menu
    _menu = new_menu(_menuItems);
    _window = newwin(windowHeight, windowWidth, _topPadding, _horizontalPadding);
    keypad(_window, true);

    // associate menu with window
    set_menu_win(_menu, _window);

    // create sub window for menu
    set_menu_sub(_menu, derwin(_window, menuHeight, menuWidth, 1, 1));

    // create sub window for menu
    set_menu_format(_menu, menuHeight, 1);

    // hide marking char for current menu selection
    set_menu_mark(_menu, "");

    // draw simple box sround window
    box(_window, 0, 0);

    // print heading and user tips
    mvprintw(0, _horizontalPadding, "%s", "Author: Wolanski Grzegorz");
    mvprintw(_topPadding - 1, _horizontalPadding + 1, "%s", "DINING TABLE STATE");
    mvprintw(_topPadding + windowHeight, _horizontalPadding, "%s", "Q - exit program");
    refresh();

    // generate menu with items in the memory of the window
    post_menu(_menu);
    wrefresh(_window);
    refresh();
}

void Graphics::refreshMenu()
{
    // save current selection so screen will not revert
    // to the top after update
    ITEM* currentItem = _menu->curitem;

    unpost_menu(_menu);
    set_menu_items(_menu, _menuItems);
    set_current_item(_menu, currentItem);
    post_menu(_menu);
    wrefresh(_window);
}

void Graphics::display()
{
    int32_t option;
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
            case 'q':
                quit = true;
                break;
            default:
                break;
        }

        wrefresh(_window);
    }
}

void Graphics::updateRow(uint32_t philosopherIndex,
                         uint32_t rightForkIndex, const std::vector<std::string>& philosopherRowValues,
                                                  const std::pair<std::string, std::string>& rightForkOwnerAndState)
{
    std::lock_guard<std::mutex> lock(mutex);

    // align indexes to menu items indexing
    philosopherIndex += _baseMenuItemId;
    rightForkIndex += _baseMenuItemId;

    // update table row with right fork of corresponding philosopher that is being updated
    _rowsValues[rightForkIndex][_rowsForkOwnerIndex] = rightForkOwnerAndState.first;
    _rowsValues[rightForkIndex][_rowsForkStateIndex] = rightForkOwnerAndState.second;

    // create new rows from given values
    std::string updatedPhilosopherRow = createRow(philosopherRowValues);
    std::string updatedRightForkRow = createRow(_rowsValues[rightForkIndex]);

    // overwrite rows at given indexes
    _rowsValues[philosopherIndex] = philosopherRowValues;
    _rows[philosopherIndex] = updatedPhilosopherRow;
    _rows[rightForkIndex] = updatedRightForkRow;
    _menuItems[philosopherIndex] = new_item(_rows[philosopherIndex].c_str(), _rows[philosopherIndex].c_str());
    _menuItems[rightForkIndex] = new_item(_rows[rightForkIndex].c_str(), _rows[rightForkIndex].c_str());

    // update menu items
    refreshMenu();
}
