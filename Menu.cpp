// Menu.cpp

#include "Menu.h"
#include "Design.h"
#include "Database.h"
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <ncurses.h>
#include <iostream>
#include "tools.h"

using namespace std;

//////////////////////////ncurses///////////////////////

void start(){

    initscr();
    noecho();
    curs_set(0);
    
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    box(win, 0, 0);

    Menu menus[3] = {
        Menu("File", 'f'),
        Menu("Edit", 'e'),
        Menu("Options", 'o')
    };

    MenuBar menubar = MenuBar(win, menus, 3);
    menubar.draw();
    
    // mvwprintw(win, 0, 2, "File");
    // mvwprintw(win, 0, 7, "Edit");
    // mvwprintw(win, 0, 12, "Options");

    
    while (true){
        char ch = wgetch(win);
        menubar.handleTrigger(ch);
        menubar.draw();
    }

    endwin();   

}


/////////////////////iosteam//////////////////////

void start_iostream(){

    cout << "Welcome to the eSport Team Database!\n";
    cout << "-----------------------------\n\n";

    Database database;

    askMenu(database);

} 

void askMenu(Database& database){

    while (true){
        cout << "-------Menu-------\n";
        cout << "(A)dd a song.\n"
                "(F)ind a song.\n"
                "(D)elete a song.\n"
                "(L)ist songs.\n"
                "(Q)uit\n\n";

        cout << "Enter the letter of your choice: ";

        string input;
        cin >> input;
        cmpt::to_lower(input);
        if (input == "a"){
            addEntry(database);
        } else if (input == "f"){
            
        } else if (input == "d"){
            
        } else if (input == "l"){
            
        } else if (input == "q"){ 
            cout << "Thanks for running the program! See you soon!\n";
            break;
        } else {
            cout << "Input was not valid! Try again!\n";
        }
    }
}

void addEntry(Database& database) {
    Team temp;
    cout << "So you want to add a team? " << endl;
    string fullName = getFullName(database);
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
    // string president = getPresident();
    // //cout << president << endl;
    // temp.set_president(president);
    // string CEO = getCEO();
    // //cout << CEO << endl;
    // temp.set_ceo(CEO);
    // vector<string> sponsors = getPartners();
    // // for(string t : sponsors) {
    // //     cout << t << endl;
    // // }
    // temp.set_partner(sponsors);
    string location = getLocation();
    //cout << location << endl;
    temp.set_location(location);
    int yearFounded = getYearFounded();
    //cout << yearFounded << endl;
    temp.set_dateFounded(yearFounded);
    database.add_team(temp);

    for(Team t : database.get_database()) {
        printEntries(t);
    }

}

void printEntries(Team t) {
    cout << "Team Full Name: " << t.get_full() << endl;
    cout << "Team Short Name: " << t.get_short() << endl;
    vector<string> v = t.get_divList();
    cout << "Games Played by Team:\n";
    for (string s : v) {
        cout << s << endl;
    }
    // cout << "Team President: " << t.get_president() << endl;
    // cout << "Team CEO: " << t.get_ceo() << endl;
    // vector<string> p = t.get_partners();
    // cout << "Partners of Team:\n";
    // for (string str : p) {
    //     cout << str << endl;
    // }
    cout << "Team Location: " << t.get_location() << endl;
    cout << "Team Date Founded: " << t.get_dateFounded() << endl;
}

string getFullName(const Database& database) {
    cout << "Please enter team name: ";
    string userInput;
    getline(cin, userInput);
    bool exists = false;
    for(Team t : database.get_database()) {
        if(t.get_full() == userInput) {
            cout << "That team already exists. Try again." << endl;
            exists = true;
            break;
        }
    }
    if(exists) {
        getFullName(database);
    }
    return userInput;
};

string getShortName() {
    cout << "Please enter your team's short name: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};

vector<string> getDivList() {
    cout << "Which games does your team participate in?" << endl;
    cout << "When you have finished entering your list, please enter \"X\" to indicate so." << endl;
    vector<string> gamesPlayed;
    //vector<string> eligibleGames; //placeholder for now; will probably fix later.
    string userInput;
    getline(cin, userInput);
    string hold = userInput;
    cmpt::to_lower(hold);
    while(hold != "x") {
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
        hold = userInput;
        cmpt::to_lower(hold);
    }
    cout << "Ok. Here are your team's games played: " << endl;
    for(string s : gamesPlayed) {
        cout << s << endl;
    }
    return gamesPlayed;
};

// string getPresident() {
//     cout << "Please enter your team's president: ";
//     string userInput;
//     getline(cin, userInput);
//     return userInput;
// }
    
// string getCEO() {
//     cout << "Please enter your team's CEO: ";
//     string userInput;
//     getline(cin, userInput);
//     return userInput;
// }
    
// vector<string> getPartners() {
//     cout << "Who are your team's sponsors? " << endl;
//     cout << "When you have finished entering your list, please enter \"X\" to indicate so." << endl;
//     vector<string> sponsors;
//     string userInput;
//     getline(cin, userInput);
//     string hold = userInput;
//     cmpt::to_lower(hold);
//     while(hold != "x") {
//         if(userInput == "") {
//             cout << "You have entered an empty field. Please try again. " << endl;
//         } else {
//             sponsors.push_back(userInput);
//             cout << "Sponsor added." << endl;
//         }
//         getline(cin, userInput);
//         hold = userInput;
//         cmpt::to_lower(hold);
//     }
//     cout << "Ok. Here are your team's sponsors: " << endl;
//     for(string s : sponsors) {
//         cout << s << endl;
//     }
//     return sponsors;
// }
    
string getLocation() {
    cout << "Please enter your team's home base/location: ";
    string userInput;
    getline(cin, userInput);
    return userInput;
};
    
int getYearFounded() {
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

    int num = stoi(cmpt::trim(userInput));
    return num;
};


