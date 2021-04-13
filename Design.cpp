// design.cpp 

#include "Design.h"
#include <ncurses.h>
#include <string>

using namespace std;

/////////////////Menu/////////////////////
Menu::Menu(string text, char trigger){
    this->text = text;
    this->trigger = trigger;
}

////////////////MenuOut///////////////////
MenuOut::MenuOut(WINDOW* win, Menu* menus, int num_menus){
    this->win = win;
    this->menus = menus;
    this->num_menus = num_menus;

    int start = win->_maxx / 6;
    int current_pos = start;

    for (int i = 0; i < num_menus; i++){
        this->menus[i].start_x = current_pos;
        if (num_menus / 2  == i + 1){
            current_pos +=  start - this->menus[i + 1].text.length()/2;
        } else {
            current_pos += start;
        }
    }
}

void MenuOut::reset(){
    for (int i = 0; i < num_menus; i++){
        int start_x = this->menus[i].start_x;
        string text = this->menus[i].text;
        mvwprintw(win, 0, start_x, text.c_str());
    }
    wrefresh(win);
}

void MenuOut::draw(){
    for (int i = 0; i < num_menus; i++){
        drawMenu(menus[i], selected_menu == i);
    }

    int center = win->_maxx / 2;
    int adj = center;

    mvwprintw(win, 5, adj, "c");
    selected_menu = -1;
}

void MenuOut::drawMenu(Menu menu, bool is_selected){
    int start_x = menu.start_x;
    string text = menu.text;
    if (is_selected){
        wattron(win, A_STANDOUT);
    }
    mvwprintw(win, 0, start_x, text.c_str());
    wattroff(win, A_STANDOUT);
    wrefresh(win);
}

void MenuOut::handleTrigger(char trigger){
    for (int i = 0; i < num_menus; i++){
        if (trigger == this->menus[i].trigger){
            selected_menu = i;
        }
    }
}



