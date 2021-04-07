// Team.cpp 

#include "Team.h"
#include <string>
#include <vector>

using namespace std;

Team::Team()
{ }

///////////////////GETTERS////////////////////////
string Team::get_full() const { return fullName; };
string Team::get_short() const { return shortName; };
vector<string> Team::get_divList() const { return divList; };
// string Team::get_president() const { return president; };
// string Team::get_ceo() const { return ceo; };
// vector<string> Team::get_partners() const { return partners; };
string Team::get_location() const { return location; };
int Team::get_dateFounded() const { return dateFounded; };


///////////////////SETTERS////////////////////////
void Team::set_full(string s) { 
    fullName = s;
}

void Team::set_short(string s){
    shortName = s;
}

void Team::set_divList(vector<string> v){
    for(string s : v) {
        divList.push_back(s);
    }
}   

// void Team::set_president(string s){
//     president = s;
// }

// void Team::set_ceo(string s){
//     ceo = s;
// }

// void Team::set_partner(vector<string> v){
//     for(string s : v) {
//         partners.push_back(s);
//     }
// }

void Team::set_location(string s){
    location = s;
}

void Team::set_dateFounded(int i){
    dateFounded = i;
}