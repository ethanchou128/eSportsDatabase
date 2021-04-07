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

////////////////MenuBar////////////////////////
MenuBar::MenuBar(WINDOW* win, Menu* menus, int num_menus){
    this->win = win;
    this->menus = menus;
    this->num_menus = num_menus;

    int current_pos = 2;

    for(int i = 0; i < num_menus; i++){
        this-> menus[i].start_x = current_pos;
        current_pos += this-> menus[i].text.length() + 1;
    }

}

void MenuBar::draw(){
    for (int i = 0; i < num_menus; i++){
        int start_x = this->menus[i].start_x;
        string text = this->menus[i].text;
        if (selected_menu == i){
            wattron(win, A_STANDOUT);
        }
        mvwprintw(win, 0, start_x, text.c_str());
        wattroff(win, A_STANDOUT);
    }
}

void MenuBar::handleTrigger(char trigger){
    for (int i = 0; i < num_menus; i++){
        if (trigger == this->menus[i].trigger){
            selected_menu = i;
        }
    }
}

