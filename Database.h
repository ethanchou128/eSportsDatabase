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

class Database {

    vector<Team> database;

public:
    
    Database();
    Database(string fileName);

    const vector<Team>& get_database() const;
    void replaceVector(vector<Team> t);
    void add_team(Team t);
    int get_size() const;

    vector<Team> get_by_name(string s) const;
    vector<Team> get_by_game(string s) const;
    vector<Team> get_by_location(string s) const;
    vector<Team> get_by_dateFounded(int n) const;

    //////////////File Handling////////////
    void save_to_file(string fileName);
    void read_from_file(string fileName);

    //////////////Sorting/////////////////
    void sort_by_name();
    void sort_by_revname();
    void sort_by_location();
    void sort_by_revlocation();
    void sort_by_netWorth();
    void sort_by_revnetWorth();
    void sort_by_yearFounded();
    void sort_by_revyearFounded();
    
    

private: 
    //////////////File Handling////////////
    void read_line(const string& line);

    //////////////Sorting/////////////////
    static bool sortByFullName(const Team &left, const Team &right);
    static bool sortByLocation(const Team &left, const Team &right);
    static bool sortByNetWorth(const Team &left, const Team &right);
    static bool sortByYearFounded(const Team &left, const Team &right);
    static bool reverseSortFullName(const Team &left, const Team &right);
    static bool reverseSortLocation(const Team &left, const Team &right);
    static bool reverseSortNetWorth(const Team &left, const Team &right);
    static bool reverseSortYearFounded(const Team &left, const Team &right);

};




#endif