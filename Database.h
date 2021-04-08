// Database.h

#ifndef DATABASE_H
#define DATABASE_H

#include "tools.h"
#include "divisions.h"
#include "Team.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

//vector<Team> readFile(string input);

class Database {

    vector<Team> database;

public:
    
    Database();

    vector<Team> get_database() const;
    void replaceVector(vector<Team> t);
    void add_team(Team t);
    int get_size() const;
    vector<Team> get_by_name(string s) const;
    vector<Team> get_by_game(string s) const;
    // vector<Team> get_by_president(string s) const;
    // vector<Team> get_by_ceo(string s) const;
    // vector<Team> get_by_partners(string s) const;
    vector<Team> get_by_location(string s) const;
    vector<Team> get_by_dateFounded(int n) const;
    
};




#endif