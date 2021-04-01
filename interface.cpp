// interface.cpp

#include "interface.h"
#include <iostream>

using namespace std;

void start(){

    initscr();

    printw("hello world");

    refresh();

    getch();

    endwin();   ;

    cout << endl;

}

