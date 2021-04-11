// Team.cpp 

#include "Team.h"
#include <string>
#include <vector>
#include "tools.h"

using namespace std;

Team::Team()
{ }

///////////////////GETTERS////////////////////////
string Team::get_full() const { return fullName; };
string Team::get_short() const { return shortName; };
vector<string> Team::get_divList() const { return divList; };
string Team::get_location() const { return location; };
int Team::get_dateFounded() const { return dateFounded; };
float Team::get_netWorth() const { return netWorth; };


///////////////////SETTERS////////////////////////
void Team::set_full(string s) { 
    fullName = cmpt::trim(s);
}

void Team::set_short(string s){
    shortName = cmpt::trim(s);
}

void Team::set_divList(vector<string> v){
    for(string s : v) {
        divList.push_back(s);
    }
}

void Team::add_to_divList(string s){
    divList.push_back(s);
}

void Team::set_location(string s){
    location = cmpt::trim(s);
}

void Team::set_dateFounded(int i){
    dateFounded = i;
}

void Team:: set_netWorth(float i) {
    netWorth = i;
}