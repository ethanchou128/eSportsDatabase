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

    Menu(string txt, char trigger);

};


class MenuBar {

public:
    WINDOW* win;
    Menu* menus;
    int num_menus;
    int selected_menu;

    MenuBar(WINDOW* win, Menu* menus, int num_menus);

    void draw();

    void handleTrigger(char trigger);
};

#endif