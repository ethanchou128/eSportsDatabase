// Database.cpp 


#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <vector>
#include <string>

using namespace std;

// vector<Team> readFile(string input){
//     ifstream inFile(input);
//     if (inFile.fail()){
//         string userInput = "";
//         cout << "Unable to open input file: " << input << ". Does not exist." << endl;
// 		exit(EXIT_FAILURE);
//     }
//     vector<Team> wordlist;
//     while (true){
//         string word;
//         inFile >> word; //words are read in
//         if (inFile.fail()){//once everything has been read in, loop is broken
//             break;
//         }
//         //wordlist.push_back(word); //words are added to vector
//     }
//     inFile.close(); //file closed
//     return wordlist;
//}

Database::Database() 
{ }

vector<Team> Database::get_database() const {
    return database;
}
void Database::replaceVector(vector<Team> t) {
    database = t;
}

void Database::add_team(Team t) {
    database.push_back(t);
}

int Database::get_size() const{
    return database.size();
}

///////////////////Searching////////////////////////
vector<Team> Database::get_by_name(string s) const{
    vector<Team> hold;
    for (Team t : database){
        string full = t.get_full();
        if (t.get_full().find(s) != string::npos || t.get_short().find(s) != string::npos){
            hold.push_back(t);
        }
        
    }
    return hold;
}

vector<Team> Database::get_by_game(string s) const{
    vector<Team> hold;
    for (Team t : database){
        for (string str : t.get_divList()){
            string clean = cmpt::clean(str);
            if (clean.find(s) != string::npos){
                hold.push_back(t);
                break;
            }
        }
    }
    return hold;
}

// vector<Team> Database::get_by_president(string s) const{

// }

// vector<Team> Database::get_by_ceo(string s) const{

// }

// vector<Team> Database::get_by_partners(string s) const{

// }

vector<Team> Database::get_by_location(string s) const{
    vector<Team> hold;
    for (Team t : database){
        if (t.get_location().find(s) != string::npos){
            hold.push_back(t);
        }
    }
    return hold;
}

vector<Team> Database::get_by_dateFounded(int n) const{
    vector<Team> hold;
    for (Team t : database){
        if (t.get_dateFounded() == n){
            hold.push_back(t);
        }
    }
    return hold;
}

