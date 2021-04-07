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

using namespace std;

//vector<Team> readFile(string input);

class Database {

    vector<Team> database;

public:
    
    Database();

    vector<Team> get_database() const;
    void add_team(Team t);
    int get_size() const;
    vector<Team> get_by_fullName(string s) const;
    vector<Team> get_by_shortName(string s) const;
    vector<Team> get_by_game(string s) const;
    // vector<Team> get_by_president(string s) const;
    // vector<Team> get_by_ceo(string s) const;
    // vector<Team> get_by_partners(string s) const;
    vector<Team> get_by_location(string s) const;
    vector<Team> get_by_dateFounded(int n) const;
    
};




#endif