#pragma once

#include <menu.h>
#include <ncurses.h>
#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>

class Graphics
{
    public:
    Graphics(const std::vector<std::string>& columnsNames,
             const std::vector<std::vector<std::string>>& rowsValues);
    ~Graphics();

    void display();
    void updateRow(uint32_t philosopherIndex,
                   uint32_t rightForkIndex, const std::vector<std::string>& philosopherRowValues,
                                            const std::pair<std::string, std::string>& rightForkOwnerAndState);


    private:
    std::string createRow(const std::vector<std::string>& columnsValues);
    void createRows();
    uint32_t calculateWindowWidth();
    uint32_t calculateWindowHeight();
    uint32_t calculateColumnWidth();
    void initMenuItems();
    void init();
    void refreshMenu();

    WINDOW* _window;
    MENU* _menu;
    ITEM** _menuItems;

    std::mutex mutex;
    std::vector<std::string> _columnsNames;
    std::vector<std::string> _rows;
    std::vector<std::vector<std::string>> _rowsValues;
    const std::string _nullRow;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPadding;
    uint32_t _columnsCount;
    uint32_t _columnWidth;
    uint32_t _rowsCount;
    const uint32_t _baseMenuItemId;
    const uint32_t _rowsForkStateIndex;
    const uint32_t _rowsForkOwnerIndex;
};
