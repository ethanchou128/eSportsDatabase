//Design.h

#include <ncurses.h>
#include <string>

#ifndef DESIGN_H
#define DESIGN_H

using namespace std;

class Menu {

public:
    int start_x;
    string text;
    char trigger;

    Menu(string txt, char trigger, string* items, int num_items);

    //selected items
    string* items;
    int num_items;
    int selected_item;

    void selectNextItem();
    void selectPreviousItem();

};


class MenuBar {

public:
    WINDOW* menuwin;
    WINDOW* win;
    Menu* menus;
    int num_menus;
    int selected_menu;

    MenuBar(WINDOW* win, Menu* menus, int num_menus);

    void reset();
    void draw();
    void drawMenu(Menu menu, bool is_selected);
    void drawMenuItems(Menu menu);

    void handleTrigger(char trigger);
};

#endif