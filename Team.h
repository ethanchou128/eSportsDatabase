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
    string president;
    string ceo;
    vector<string> partners;
    string location;
    int dateFounded;

public:
    Team();

    string get_full const;
    string get_short const;
    vector<string> get_divList const;
    string get_president const;
    string get_ceo const;
    vector<string> get_partners const;
    string get_location const;
    int get_dateFounded const;

    void set_full; 
    void set_short;
    void set_divList;
    void set_president;
    void set_ceo;
    void set_partner;
    void set_location;
    void set_dateFounded;
}



#endif