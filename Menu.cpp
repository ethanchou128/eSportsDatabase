// Menu.cpp

#include "Menu.h"
#include "tools/Design.h"
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <ncurses.h>

using namespace std;

void start(){

    initscr();
    noecho();
    curs_set(0);
    
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    box(win, 0, 0);

    Menu menus[3] = {
        Menu("File", 'f'),
        Menu("Edit", 'e'),
        Menu("Options", 'o')
    };

    MenuBar menubar = MenuBar(win, menus, 3);
    menubar.draw();
    
    // mvwprintw(win, 0, 2, "File");
    // mvwprintw(win, 0, 7, "Edit");
    // mvwprintw(win, 0, 12, "Options");

    
    while (true){
        char ch = wgetch(win);
        switch (ch){
            case 'f' :
                wattron(win, A_STANDOUT);
                mvwprintw(win, 0, 2, "File");
                wattroff(win, A_STANDOUT);
                break; 
            case 'e' :
                break;
            case 'o' :
                break;
            default:
                mvwprintw(win, 0, 2, "File");
                mvwprintw(win, 0, 7, "Edit");
                mvwprintw(win, 0, 12, "Options");
                break;
        }
    }

    endwin();   

}

