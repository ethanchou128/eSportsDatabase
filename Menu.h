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

// Menu:
void askMenu(Database& database);
void addEntry(Database& database);
void findEntry(Database& database);

//Printing:
void printEntries(Database& database);
void printEntries(const vector<Team>& vT);
void printEntries(const Team& t);

//Adding:
string getFullName(const Database& database);
string getShortName();
vector<string> getDivList();
string getPresident();
string getCEO();
vector<string> getPartners();
string getLocation();
int getYearFounded();
float getNetWorth();

// Delete:
void deleteEntry(Database &database);
void listEntries(Database &database);

// Listing:
bool sortByFullName(const Team &left, const Team &right);
bool sortByLocation(const Team &left, const Team &right);
bool sortByNetWorth(const Team &left, const Team &right);
bool sortByYearFounded(const Team &left, const Team &right);
bool reverseSortFullName(const Team &left, const Team &right);
bool reverseSortLocation(const Team &left, const Team &right);
bool reverseSortNetWorth(const Team &left, const Team &right);
bool reverseSortYearFounded(const Team &left, const Team &right);

#endif