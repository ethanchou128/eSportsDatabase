// Database.cpp 


#include "Database.h"
#include "Team.h"
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

void Database::add_team(Team t) {
    database.push_back(t);
}

int Database::get_size() const{
    return database.size();
}

///////////////////Searching////////////////////////
vector<Team> Database::get_by_fullName(string s) const{
    vector<Team> hold;
    for (Team t : database){
        if (t.get_full() == s){
            hold.push_back(t);
        }
    }
    return hold;
}

vector<Team> Database::get_by_shortName(string s) const{
    vector<Team> hold;
    for (Team t : database){
        if (t.get_short() == s){
            hold.push_back(t);
        }
    }
    return hold;
}

vector<Team> Database::get_by_game(string s) const{

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
        if (t.get_location() == s){
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

