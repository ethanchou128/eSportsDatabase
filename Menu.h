// Menu.h

#ifndef MENU_H
#define MENU_H

#include "Design.h"
#include "Database.h"
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <ncurses.h>
#include <iostream>
#include <vector>

using namespace std;

/////////////////////ncurses//////////////////////
void start();

/////////////////////iosteam//////////////////////
void start_iostream();
void askMenu(Database& database);
void addEntry(Database& database);
void printEntries(Team t);
string getFullName(const Database& database);
string getShortName();
vector<string> getDivList();
string getPresident();
string getCEO();
vector<string> getPartners();
string getLocation();
int getYearFounded();
double getNetWorth();

//////////////////delete entry///////////////////
void deleteEntry(Database &database);

#endif