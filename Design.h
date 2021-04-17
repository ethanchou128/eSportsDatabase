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
    /* Main Menu System */
    int start_x_menu;
    string text;
    char trigger;
    
    /* Sub-Menu Systems */
    Menu(string text, char trigger, string* options, char* options_trigger, int num_options);

    int start_x_options;
    string* options;
    char* options_trigger;
    int num_options;
    bool selected;
    int selected_option;
    bool reversed;
    

    void selectNextOption();
    void selectPreviousOption();


};

class MenuOut{

public:
    /* Sub-Menu Systems */
    WINDOW* optionwin;

    /* Main Menu system */
    WINDOW* win;
    Menu* menus;
    int num_menus;
    int selected_menu;
    bool selected;
    bool run;
    Database database;
    

    MenuOut(WINDOW* win, Menu* menus, int num_menus);
    
    /* Higher Order Function */
    // Called in Menu
    void draw();
    void handleTriggerMenu(int trigger);

    /* Main Drawing */
    void reset();
    void menureset(Menu& menu);
    void drawMenu(Menu menu, bool is_selected);
    void print_centered(WINDOW* win, int start_row, string text);
    void drawMenuOptions(Menu& menu);

    /* User Input Handle */
    void handleTriggerOptions(Menu& menu, int trigger);
    void selectNextMenu();
    void selectPreviousMenu();

    /* Printing */
    void printEntries(WINDOW* win, Database& database);
    void printEntries(WINDOW* win, const vector<Team>& vT);
    void printEntries(WINDOW* win, const Team& t);

    /* Adding */
    void addEntry(Menu& menu);
    string getaddinput(Menu& menu);

    /* Deleting */
    void deleteEntry(Menu& menu);

    /* Finding */
    void findMenu(Menu& menu);

    /* Listing */
    void listMenu(Menu& menu);


};

#endif