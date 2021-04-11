// Database.cpp 


#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

using namespace std;

Database::Database() 
{ }

Database::Database(string fileName){
    read_from_file(fileName);
}

const vector<Team>& Database::get_database() const {
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

/////////////////////////////////////////////////////////
// File Handling
/////////////////////////////////////////////////////////
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

//Specific to files created by save_to_file
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

/////////////////////////////////////////////////////////
// sorting
/////////////////////////////////////////////////////////
bool Database::sortByFullName(const Team &left, const Team &right) {
    return left.get_full() < right.get_full();
}

bool Database::sortByLocation(const Team &left, const Team &right) {
    // sort by two fields
    // when the locations are the same
    // sorts team alphabetically
    if (left.get_location() < right.get_location()){
        return true;
    }
    if (right.get_location() < left.get_location()){
        return false;
    }
    if (left.get_full() < right.get_full()){
        return true;
    }
    if (right.get_full() < left.get_full()){
        return false;
    }
    return false;
}

bool Database::sortByNetWorth(const Team &left, const Team &right) {
    return left.get_netWorth() < right.get_netWorth();
}

bool Database::sortByYearFounded(const Team &left, const Team &right) {
    if (left.get_dateFounded() < right.get_dateFounded()){
        return true;
    }
    if (right.get_dateFounded() < left.get_dateFounded()){
        return false;
    }
    if (left.get_full() < right.get_full()){
        return true;
    }
    if (right.get_full() < left.get_full()){
        return false;
    }
    return false;
}

bool Database::reverseSortFullName(const Team &left, const Team &right) {
    return left.get_full() > right.get_full();
}

bool Database::reverseSortLocation(const Team &left, const Team &right) {
    if (left.get_location() > right.get_location()){
        return true;
    }
    if (right.get_location() > left.get_location()){
        return false;
    }
    if (left.get_full() < right.get_full()){
        return true;
    }
    if (right.get_full() < left.get_full()){
        return false;
    }
    return false;
}

bool Database::reverseSortNetWorth(const Team &left, const Team &right) {
    return left.get_netWorth() > right.get_netWorth();
}

bool Database::reverseSortYearFounded(const Team &left, const Team &right) {
    if (left.get_dateFounded() > right.get_dateFounded()){
        return true;
    }
    if (right.get_dateFounded() > left.get_dateFounded()){
        return false;
    }
    if (left.get_full() < right.get_full()){
        return true;
    }
    if (right.get_full() < left.get_full()){
        return false;
    }
    return false;
}


void Database::sort_by_name(){
    sort(database.begin(), database.end(), sortByFullName);
}
void Database::sort_by_revname(){
    sort(database.begin(), database.end(), reverseSortFullName);
}
void Database::sort_by_location(){
    sort(database.begin(), database.end(), sortByLocation);
}
void Database::sort_by_revlocation(){
    sort(database.begin(), database.end(), reverseSortLocation);
}
void Database::sort_by_netWorth(){
    sort(database.begin(), database.end(), sortByNetWorth);
}
void Database::sort_by_revnetWorth(){
    sort(database.begin(), database.end(), reverseSortNetWorth);
}
void Database::sort_by_yearFounded(){
    sort(database.begin(), database.end(), sortByYearFounded);
}
void Database::sort_by_revyearFounded(){
    sort(database.begin(), database.end(), reverseSortYearFounded);
}
