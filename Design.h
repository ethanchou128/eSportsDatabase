//Design.h

#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <iostream>
#include <algorithm>
#include <vector>

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
    bool selected;
    bool run;
    Database database;
    

    MenuOut(WINDOW* win, Menu* menus, int num_menus);

    void reset();
    void draw();
    void drawMenu(Menu menu, bool is_selected);
    void print_centered(WINDOW* win, int start_row, string text);

    void handleTriggerMenu(int trigger);
    void selectNextMenu();
    void selectPreviousMenu();

    void printEntries(Database& database);
    void printEntries(const vector<Team>& vT);
    void printEntries(const Team& t);
};

#endif