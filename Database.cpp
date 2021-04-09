// Database.cpp 


#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <vector>
#include <string>
#include <fstream>

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

Database::Database(string fileName){
    read_from_file(fileName);
}

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

//////////////////////File Handling//////////////////////
void Database::save_to_file(string fileName){

    ofstream file;
    file.open(fileName);

    if (file.is_open()){;

        if (!database.empty()){;
        
            for (Team t : database){;
                file << t.get_full() << ", ";
                file << t.get_short() << ", ";
                file << "{";
                for (int i = 0; i < t.get_divList().size(); i++){
                    if (i != t.get_divList().size() - 1){
                        file << t.get_divList().at(i) << ", ";
                    } else {
                        file << t.get_divList().at(i);
                    }
                }
                file << "}, ";
                file << t.get_location() << ", ";
                file << t.get_dateFounded() << ", ";
                file << t.get_netWorth() << "\n";
            }

        }

    }


    file.close();

}

void Database::read_from_file(string fileName){
    ifstream file;
    file.open(fileName);

    if (file.is_open()){
        string line;
        while(getline(file, line)){
            read_line(line);
        }
    }
    file.close();
}

void Database::read_line(const string& line){

    string field;
    int fieldNum = 1;
    Team temp;
    for (int i = 0; i < line.size(); i++){

        if (line.at(i) != ',' && i != line.size() - 1 && line.at(i) != '{' && line.at(i) != '}'){

            field.push_back(line.at(i));

        } else if (i == line.size() - 1){
            field.push_back(line.at(i));
            temp.set_netWorth(stod(cmpt::trim(field)));
        } else if (line.at(i) == ',' && line.at(i - 1) != '}'){
            field = cmpt::trim(field);
            switch (fieldNum){
                case 1:
                    temp.set_full(field);
                    fieldNum++;
                    break;
                case 2: 
                    temp.set_short(field);
                    fieldNum++;
                    break;
                case 3:
                    temp.add_to_divList(field);
                    break;
                case 4:
                    temp.set_location(field);
                    fieldNum++;
                    break;
                case 5:
                    temp.set_dateFounded(stoi(field));
                    fieldNum++;
                    break;
                
            }
            field = "";
        } else if (line.at(i - 1) == '}' && line.at(i) == ','){
            temp.add_to_divList(cmpt::trim(field));
            fieldNum++;
            field = "";
        }

    }

    database.push_back(temp);

}

