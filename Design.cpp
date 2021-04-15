// design.cpp 

#include "Design.h"
#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <algorithm>
#include <vector>

using namespace std;

/////////////////Menu/////////////////////
Menu::Menu(string text, char trigger, string* options, char* options_trigger, int num_options){
    this->text = text;
    this->trigger = trigger;
    this->options = options;
    this->options_trigger = options_trigger;
    this->num_options = num_options;
    this->selected_option = -1;
    this->selected = false;
}

void Menu::selectNextOption(){
    selected_option++;
    if (selected_option >= num_options){
        selected_option = 0;
    }
}

void Menu::selectPreviousOption(){
    selected_option--;
    if (selected_option < 0){
        selected_option = num_options - 1;
    }
}

////////////////MenuOut///////////////////
MenuOut::MenuOut(WINDOW* win, Menu* menus, int num_menus){
    this->win = win;
    this->menus = menus;
    this->num_menus = num_menus;
    this->selected_menu = -1;
    this->selected = false;
    this->run = true;
    Database database("database.txt");
    this->database = database;

    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);
    optionwin = newwin(yMax, xMax, yBeg, xBeg);
    keypad(optionwin, true);
    wrefresh(win);

    int start = getmaxx(win) / (num_menus + 1);
    int current_pos;

    for (int i = 0; i < num_menus; i++){
        int start_options = getmaxx(win) / (this->menus[i].num_options + 1);
        current_pos = start + (start * i) - this->menus[i].text.length()/2;
        this->menus[i].start_x_menu = current_pos;
        this->menus[i].start_x_options = start_options;
    }
}

void MenuOut::reset(){
    werase(win);
    box(win, 0, 0);
    wrefresh(win);
    for (int i = 0; i < num_menus; i++){
        int start_x = this->menus[i].start_x_menu;
        string text = this->menus[i].text;
        mvwprintw(win, 0, start_x, text.c_str());
    }
    
    print_centered(win, 2, "Welcome to Justin and Ethan's Database of Esports Teams");
    print_centered(win, 3, "Press the letter that corresponds to the action to select it");
    print_centered(win, 4, "Alternatively you can use the arrow keys go through the menu");
    print_centered(win, 5, "Press enter once you have selected an option");

    wrefresh(win);
}

void MenuOut::draw(){
    if (!selected){
        for (int i = 0; i < num_menus; i++){
            drawMenu(menus[i], selected_menu == i);
        }
    } else if (selected && selected_menu == num_menus - 1){
        run = false;
    } else {
        int ch;
        drawMenuOptions(menus[selected_menu]);
        wrefresh(win);
        while(selected && (ch = wgetch(optionwin))){
            handleTriggerOptions(menus[selected_menu], ch);
            if (menus[selected_menu].selected){
                if (menus[selected_menu].selected_option == menus[selected_menu].num_options - 1 
                    && menus[selected_menu].selected == true){
                    menus[selected_menu].selected = false;
                    selected = false;
                }
            }
            drawMenuOptions(menus[selected_menu]);
        }
        werase(optionwin);
        wrefresh(optionwin);
        reset();
        selected = false;
    }
}

void MenuOut::drawMenu(Menu menu, bool is_selected){
    int start_x = menu.start_x_menu;
    string text = menu.text;
    if (is_selected){
        wattron(win, A_STANDOUT);
    }
    mvwprintw(win, 0, start_x, text.c_str());
    wattroff(win, A_STANDOUT);
    wrefresh(win);

}

void MenuOut::print_centered(WINDOW* win, int start_row, string text){
    int center = getmaxx(win) / 2;
    int half = text.length() / 2;
    int adj = center - half;

    mvwprintw(win, start_row, adj, text.c_str());
}

void MenuOut::drawMenuOptions(Menu menu){
    box(optionwin, 0, 0);
    int start = menu.start_x_options;
    int current_pos;
    for (int i = 0; i < menu.num_options; i++){
        if (menu.selected_option == i){
            wattron(optionwin, A_STANDOUT);
        }
        current_pos = start + (start * i) - menu.options[i].length() / 2;
        mvwprintw(optionwin, 0, current_pos, menu.options[i].c_str());
        wattroff(optionwin, A_STANDOUT);
        wrefresh(optionwin);
    }
}

void MenuOut::handleTriggerMenu(int trigger){
    if (trigger >= 10 && trigger <= 13){
        this->selected = true;
        return;
    }
    switch (trigger){
        case KEY_RIGHT:
            selectNextMenu();
            break;
        case KEY_LEFT:
            selectPreviousMenu();
            break;
        default:
            selected_menu = -1;
            break;
    }

    for (int i = 0; i < num_menus; i++){
        if (trigger == this->menus[i].trigger){
            selected_menu = i;
        }
    }
}

void MenuOut::handleTriggerOptions(Menu& menu, int trigger){
    if (trigger >= 10 && trigger <= 13){
        menu.selected = true;
        return;
    }

    switch (trigger){
        case KEY_RIGHT:
            menu.selectNextOption();
            break;
        case KEY_LEFT:
            menu.selectPreviousOption();
            break;
        default:
            menu.selected_option = -1;
            break;
    }

    for (int i = 0; i < menu.num_options; i++){
        if (trigger == menu.options_trigger[i]){
            menu.selected_option = i;
        }
    }
}

void MenuOut::selectNextMenu(){
    selected_menu++;
    if (selected_menu >= num_menus){
        selected_menu = 0;
    }
}

void MenuOut::selectPreviousMenu(){
    selected_menu--;
    if (selected_menu < 0){
        selected_menu = num_menus - 1;
    }
}

void MenuOut::printEntries(Database& database){
    database.sort_by_name();
    printEntries(database.get_database());
}

void MenuOut::printEntries(const vector<Team>& vT){
    werase(win);
    box(win, 0, 0);
    wrefresh(win);

    if (vT.empty()){
        print_centered(win, 2, "There are not records to print!");
        return;
    }
    int i = 1;
    
    for (Team t: vT){
        string txt = "Team " + i + ':';
        print_centered(win, 2, txt);
        print_centered(win, 3, "------");
        printEntries(t);
        i++;
    }
}

void MenuOut::printEntries(const Team& t) {
    print_centered(win, 4, "Team Full Name: " + t.get_full());
    print_centered(win, 5, "Team Short Name: " + t.get_short());
    print_centered(win, 6, "Games Played by Team:");
    vector<string> v = t.get_divList();
    int final;
    for (int i = 0; i < v.size(); i++){
        final = 7 + i;
        print_centered(win, final, v.at(i));
    }
    print_centered(win, final + 1, "Team location: " + t.get_location());
    print_centered(win, final + 2, "Team founded date: " + to_string(t.get_dateFounded()));

    string text = "Team Net Worth: %.2f million";
    int center = getmaxx(win) / 2;
    int half = text.length() / 2;
    int adj = center - half;
    mvwprintw(win, final + 3, adj, "Team Net Worth: %.2f million", t.get_netWorth());
}







