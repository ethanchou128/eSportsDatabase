// Menu.h

/*
Push instructions
1. git add .
2. git commit -m "message here"
3. git push -u origin ethan

Pull instructions
1. git pull origin master
*/

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
void findEntry(Database& database);
void printEntries(const vector<Team>& tV);
void printEntries(const Team& t);
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