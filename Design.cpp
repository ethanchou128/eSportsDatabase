// design.cpp 

#include "Design.h"
#include <ncurses.h>
#include <string>

using namespace std;

/////////////////Menu/////////////////////
Menu::Menu(string text, char trigger, string* items, int num_items){
    this->text = text;
    this->trigger = trigger;
    this->items = items;
    this->num_items = num_items;
    this->selected_item = 0;
}

void Menu::selectNextItem(){
    selected_item++;
    if (selected_item >= num_items){
        selected_item = 0;
    }
}

void Menu::selectPreviousItem(){
    selected_item--;
    if (selected_item < 0 ){
        selected_item = num_items - 1;
    }
}

////////////////MenuBar////////////////////////
MenuBar::MenuBar(WINDOW* win, Menu* menus, int num_menus){
    this->win = win;
    this->menus = menus;
    this->num_menus = num_menus;
    this->selected_menu = -1;

    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win , yBeg, xBeg);

    menuwin = newwin(yMax - 2, xMax - 2, yBeg + 1, xBeg + 1);
    keypad(menuwin, true);
    wrefresh(win);

    int current_pos = 2;

    for(int i = 0; i < num_menus; i++){
        this-> menus[i].start_x = current_pos;
        current_pos += this-> menus[i].text.length() + 1;
    }

}

void MenuBar::reset(){
    for (int i = 0; i < num_menus; i++){
        int start_x = this->menus[i].start_x;
        string text = this->menus[i].text;        
        mvwprintw(win, 0, start_x, text.c_str());
    }
    wrefresh(win);
}

void MenuBar::draw(){
    for (int i = 0; i < num_menus; i++){
        drawMenu(menus[i], selected_menu == i);
    }
    selected_menu = -1;
}

void MenuBar::drawMenu(Menu menu, bool is_selected){
    int start_x = menu.start_x;
    string text = menu.text;
    if (is_selected){
        wattron(win, A_STANDOUT);
    }
    mvwprintw(win, 0, start_x, text.c_str());
    wattroff(win, A_STANDOUT);
    wrefresh(win);

    int ch;
    drawMenuItems(menu);
    wrefresh(win);
    while (is_selected && (ch = wgetch(menuwin)))
    {
        switch (ch){
            case KEY_DOWN:
                menu.selectNextItem();
                break;
            case KEY_UP:
                menu.selectPreviousItem();
                break;
            default:
                is_selected = false;
        }
        drawMenuItems(menu);
    }
    werase(menuwin);
    wrefresh(menuwin);
    reset();
}

void MenuBar::drawMenuItems(Menu menu){
    int yMax, xMax;
    getmaxyx(menuwin, yMax, xMax);
    (void)yMax;
    for (int i = 0; i < menu.num_items; i++){
        mvwprintw(menuwin, i, 0, menu.items[i].c_str());
        if (menu.selected_item == i){
            mvwchgat(menuwin, i, 0, xMax, A_NORMAL, 1, NULL);
        } else {
            mvwchgat(menuwin, i, 0, xMax, A_STANDOUT, 1, NULL);
        }
    }
}

void MenuBar::handleTrigger(char trigger){
    for (int i = 0; i < num_menus; i++){
        if (trigger == this->menus[i].trigger){
            selected_menu = i;
        }
    }
}

