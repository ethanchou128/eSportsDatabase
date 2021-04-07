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

};




#endif