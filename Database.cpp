// Database.cpp 


#include "Database.h"

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
