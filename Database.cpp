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

void Database::addEntry() {
    Team temp;
    cout << "So you want to add a team? " << endl;
    string fullName = getFullName();
    //cout << fullName << endl;
    temp.set_full(fullName);
    string shortName = getShortName();
    //cout << shortName << endl;
    temp.set_short(shortName);
    vector<string> games = getDivList();
    // for(string s : games) {
    //     cout << s << endl;
    // }
    temp.set_divList(games);
    string president = getPresident();
    //cout << president << endl;
    temp.set_president(president);
    string CEO = getCEO();
    //cout << CEO << endl;
    temp.set_ceo(CEO);
    vector<string> sponsors = getPartners();
    // for(string t : sponsors) {
    //     cout << t << endl;
    // }
    temp.set_partner(sponsors);
    string location = getLocation();
    //cout << location << endl;
    temp.set_location(location);
    int yearFounded = getYearFounded();
    //cout << yearFounded << endl;
    temp.set_dateFounded(yearFounded);
    database.push_back(temp);
    for(Team t : database) {
        printEntries(t);
    }

}

void Database::printEntries(Team t) {
    cout << t.get_full() << endl;
    cout << t.get_short() << endl;
    vector<string> v = t.get_divList();
    for(string s : v) {
        cout << s << endl;
    }
    cout << t.get_president() << endl;
    cout << t.get_ceo() << endl;
    vector<string> p = t.get_partners();
    for(string str : p) {
        cout << str << endl;
    }
    cout << t.get_location() << endl;
    cout << t.get_dateFounded() << endl;
}

string Database::getFullName() const {
    cout << "Please enter team name: ";
    string userInput;
    getline(cin, userInput);
    bool exists = false;
    for(Team t : database) {
        if(t.get_full() == userInput) {
            cout << "That team already exists. Try again." << endl;
            break;
        }
    }
    if(exists) {
        getFullName();
    }
    return userInput;
};

string Database::getShortName() const {
    cout << "Please enter your team's short name: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};

vector<string> Database::getDivList() const {
    cout << "Which games does your team participate in?" << endl;
    cout << "When you have finished entering your list, please enter \"X\" to indicate so." << endl;
    vector<string> gamesPlayed;
    //vector<string> eligibleGames; //placeholder for now; will probably fix later.
    string userInput;
    getline(cin, userInput);
    while(userInput != "X") {
        // bool isEligible = false;
        // for(string str : eligibleGames) {
        //     if(userInput == str) {
        //         cout << "Game Added." << endl;
        //         gamesPlayed.push_back(userInput);
        //         isEligible = true;
        //     }
        // }
        // if(!isEligible) {
        //     cout << "You have entered an invalid or unpopular game. Please try again." << endl;
        // }
        gamesPlayed.push_back(userInput);
        getline(cin, userInput);
    }
    cout << "Ok. Here are your team's games played: " << endl;
    for(string s : gamesPlayed) {
        cout << s << endl;
    }
    return gamesPlayed;
};

string Database::getPresident() const {
    cout << "Please enter your team's president: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};
    
string Database::getCEO() const {
    cout << "Please enter your team's CEO: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};
    
vector<string> Database::getPartners() const {
    cout << "Who are your team's sponsors? " << endl;
    cout << "When you have finished entering your list, please enter \"X\" to indicate so." << endl;
    vector<string> sponsors;
    string userInput;
    getline(cin, userInput);
    while(userInput != "X") {
        if(userInput == "") {
            cout << "You have entered an empty field. Please try again. " << endl;
        } else {
            sponsors.push_back(userInput);
            cout << "Sponsor added." << endl;
        }
        getline(cin, userInput);
    }
    cout << "Ok. Here are your team's sponsors: " << endl;
    for(string s : sponsors) {
        cout << s << endl;
    }
    return sponsors;
};
    
string Database::getLocation() const {
    cout << "Please enter your team's home base/location: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};
    
int Database::getYearFounded() const {
    cout << "Please enter the year your team was founded: ";
    string userInput;
    cin >> userInput;
    bool isNum = true;
    for(int i = 0; i < userInput.length(); i++) {
        if(!isdigit(userInput.at(i))) {
            cout << "Sorry, that's an invalid entry. Please Try Again." << endl;
            isNum = false;
            break;
        }
    }
    if(!isNum) {
        getYearFounded();
    }
    int num = stoi(userInput);
    return num;
};
