// Menu.cpp

#include "Menu.h"

using namespace std;

void start(){

    initscr();

    printw("hello world");

    refresh();

    getch();

    endwin();   

    cout << endl;

}

