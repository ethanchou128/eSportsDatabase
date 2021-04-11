// Team.h

#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>

using namespace std;

class Team {
    string fullName;
    string shortName;
    vector<string> divList;
    string location;
    int dateFounded;
    float netWorth;

public:
    Team();

    string get_full() const;
    string get_short() const;
    vector<string> get_divList() const;
    string get_location() const;
    int get_dateFounded() const;
    float get_netWorth() const;

    void set_full(string s); 
    void set_short(string s);
    void set_divList(vector<string> v);
    void add_to_divList(string s);
    void set_location(string s);
    void set_dateFounded(int i);
    void set_netWorth(float i);
};  


#endif