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

    Menu(string text, char trigger);
    

};

class MenuOut{

public:
    WINDOW* win;
    Menu* menus;
    int num_menus;
    int selected_menu;

    MenuOut(WINDOW* win, Menu* menus, int num_menus);

    void reset();
    void draw();
    void drawMenu(Menu menu, bool is_selected);

    void handleTrigger(char trigger);
};

#endif