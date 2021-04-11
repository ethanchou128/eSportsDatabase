// Menu.cpp

#include "Menu.h"
#include "Design.h"
#include "Database.h"
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <ncurses.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "tools.h"

using namespace std;

//////////////////////////ncurses///////////////////////

void start(){

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    if (!has_colors()){
        return;
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* win = newwin(yMax*3/4, xMax*3/4, yMax/8, xMax/8);
    box(win, 0, 0);

    string menu1[] = {"New", "Open", "Save", "Exit"};
    string menu2[] = {"Copy", "Cut", "Paste"};
    string menu3[] = {"Sidebar", "Terminal"};

    Menu menus[3] = {
        Menu("File", 'f', menu1, 4),
        Menu("Edit", 'e', menu2, 3),
        Menu("Options", 'o', menu3, 2)
    };

    MenuBar menubar = MenuBar(win, menus, 3);
    menubar.draw();
    
    // mvwprintw(win, 0, 2, "File");
    // mvwprintw(win, 0, 7, "Edit");
    // mvwprintw(win, 0, 12, "Options");

    char ch;
    while ((ch = wgetch(win))){
        menubar.handleTrigger(ch);
        menubar.draw();
    }

    endwin();   

}

///////////////////////////////////////////////////////////////////////////////
// iostream
// 
// Make sure to change the comment code in final_project_main.cpp
//
// This will be used at the defualt menu if ncurses takes too long
// 
///////////////////////////////////////////////////////////////////////////////
void start_iostream(){

    cout << "Welcome to the eSport Team Database!\n";
    cout << "-----------------------------\n\n";

    Database database("database.txt");

    askMenu(database);

} 


///////////////////////////////////////////////////////////////////////////////
// Menu System
///////////////////////////////////////////////////////////////////////////////
void askMenu(Database& database){

    while (true){
        cout << "-------Menu-------\n";
        cout << "(A)dd a team.\n"
                "(F)ind a team.\n"
                "(D)elete a team.\n"
                "(L)ist teams.\n"
                "(Q)uit\n"
                "-------------------\n\n";

        cout << "Enter the letter of your choice: ";

        string input;
        cin >> input;
        cout << endl;
        if (cmpt::clean(input) == "a"){
            addEntry(database);
        } else if (input == "f"){
            findEntry(database);
        } else if (input == "d"){
            deleteEntry(database);
        } else if (input == "l"){
            // sort(database.begin(), database.end());
            // for(Team t : database) {
            //     printEntries(t);
            // }
            listEntries(database);
        } else if (input == "q"){ 
            database.save_to_file("database.txt");
            cout << "Thanks for running the program! See you soon!\n";
            break;
        } else {
            cout << "Input was not valid! Try again!\n";
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Printing
///////////////////////////////////////////////////////////////////////////////
void printEntries(Database& database){
    database.sort_by_name();
    printEntries(database.get_database());
}

void printEntries(const vector<Team>& vT){
    if (vT.empty()){
        cout << "There are not records to print!\n";
        return;
    }
    int i = 1;
    
    for (Team t: vT){
        cout << "Team " << i << ": \n";
        cout << "------\n";
        printEntries(t);
        cout << "\n";
        i++;
    }
}

void printEntries(const Team& t) {
    cout << "Team Full Name: " << t.get_full() << "\n";
    cout << "Team Short Name: " << t.get_short() << "\n";
    vector<string> v = t.get_divList();
    cout << "Games Played by Team:\n";
    for (string s : v) {
        cout << s << endl;
    }

    cout << "Team location: " << t.get_location() << endl;
    cout << "Team founded date: " << t.get_dateFounded() << endl;
    cout << "Team Net Worth: " << t.get_netWorth() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Adding 
///////////////////////////////////////////////////////////////////////////////
void addEntry(Database& database) {
    Team temp;
    cout << "So you want to add a team? " << endl;
    string fullName = getFullName(database);
    temp.set_full(fullName);
    string shortName = getShortName();
    temp.set_short(shortName);
    vector<string> games = getDivList();
    temp.set_divList(games);
    
    string location = getLocation();
    temp.set_location(location);
    int yearFounded = getYearFounded();
    temp.set_dateFounded(yearFounded);
    double netWorth = getNetWorth();
    temp.set_netWorth(netWorth);
    database.add_team(temp);

    printEntries(database);

}

string getFullName(const Database& database) {
    cout << "Please enter team name: ";
    cin.ignore(100, '\n');
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
    cout << "Which games does your team participate in? Enter the exact game title." << endl;
    cout << "When you have finished entering your list, please enter 0 to indicate so." << endl;
    vector<string> gamesPlayed;
    vector<string> eligibleGames;
    eligibleGames = {"Call of Duty", "CS:GO", "Fortnite", "League of Legends", "Overwatch", "Rainbow Six Siege", 
                     "Rocket League", "Super Smash Bros.", "Valorant"};
    int userInput;
    cout << "Here are the eligible games: " << endl;
    for(int i = 0; i < eligibleGames.size(); i++) {
        cout << "(" << i+1 << ")" << " " << eligibleGames.at(i) << endl;
    }
    cin >> userInput;
    while(userInput != 0) {
        bool isEligible = false;
        // for(string str : eligibleGames) {
        //     if(userInput == str) {
        //         cout << "Game Added." << endl;
        //         gamesPlayed.push_back(userInput);
        //         isEligible = true;
        //     }
        // }
        for(int i = 0; i < eligibleGames.size(); i++) {
            if(userInput - 1 == i) {
                cout << eligibleGames.at(i) << " has been added." << endl;
                gamesPlayed.push_back(eligibleGames.at(i));
                isEligible = true;
            }
        }
        if(!isEligible) {
            cout << "You have entered an invalid or unpopular game. Please try again." << endl;
        }
        //getline(cin, userInput);
        //hold = cmpt::trim(userInput);
        //cmpt::to_lower(hold);
        cin >> userInput;
    }
    sort(gamesPlayed.begin(), gamesPlayed.end());
    cout << "Ok. Here are your team's games played: " << endl;
    for(string s : gamesPlayed) {
        cout << s << endl;
    }
    return gamesPlayed;
};

string getLocation() {
    cout << "Please enter your team's region/country: ";
    string userInput;
    cin.ignore(100, '\n');
    getline(cin, userInput);
    bool isCountry = true;
    for(int i = 0; i < userInput.length(); i++) {
        if(isdigit(userInput.at(i))) {
            cout << "Sorry, that's an invalid entry. Please Try Again." << endl;
            isCountry = false;
            break;
        }
    }
    if(!isCountry) {
        getLocation();
    }
    string country = cmpt::trim(userInput);
    return country;
};
    
int getYearFounded() {
    cout << "Please enter the year your team was founded: ";
    int userInput;
    cin.ignore(100, '\n');
    cin.sync();
    cin >> userInput;
    while (cin.fail()){
        cout << "Sorry, that's an invalid entry. Please Try Again." << endl;
        cin >> userInput;
    }
    
    // if(!isNum) {
    //     getYearFounded();
    // }

    int num = userInput;
    return num;
};

float getNetWorth() {
    cout << "Please enter your team's net worth, in millions of dollars: ";
    string userInput;
    cin >> userInput;
    bool isNum = true;
    bool isDecimal = false;
    for(int i = 0; i < userInput.length(); i++) {
        if(!isdigit(userInput.at(i))) {
            if(userInput.at(i) == '.' && !isDecimal) {
                isDecimal = true;
            } else {
                cout << "Sorry, that's an invalid entry. Please Try Again." << endl;
                isNum = false;
                break;
            }
        }
    }
    if(!isNum) {
        getNetWorth();
    }
    float num = stof(cmpt::trim(userInput));
    return num;
};

///////////////////////////////////////////////////////////////////////////////
// Deleting 
///////////////////////////////////////////////////////////////////////////////
void deleteEntry(Database &database) {
    if (database.get_database().empty()){
        cout << "There are no records in the database!\n\n";
        return;
    }
    cout << "Teams in database: \n";
    for(Team f : database.get_database()) {
        cout << f.get_full() << endl;
    }
    cout << "Please enter the team name that you want to delete: ";
    cin.ignore(100, '\n');
    string userInput;
    getline(cin, userInput);
    
    bool exists = false;
    int indexCounter = 0;
    
    for(Team t : database.get_database()) {
        if(t.get_full() == userInput || t.get_short() == userInput) {
            exists = true;
            break;
        }
        indexCounter++;
    }
    if(!exists) {
        cout << "That team doesn't exist. Please try again." << endl;
        deleteEntry(database);
    } else {
        vector<Team> temp = database.get_database();
        temp.erase(temp.begin() + indexCounter);
        database.replaceVector(temp);
        for(Team f : database.get_database()) {
            cout << f.get_full() << endl;
        }
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Finding 
///////////////////////////////////////////////////////////////////////////////
void findEntry(Database& database){
    if (database.get_database().empty()){
        cout << "There are no records in the database!\n\n";
        return;
    }

    database.sort_by_name();

    while (true){
        cout << "-------Find a Team-------\n";
        cout << "(N)ame of team.\n"
                "(G)ame teams play.\n"
                "(L)ocation of teams.\n"
                "(D)ate teams were founded.\n"
                "--------------------------\n"
                "\n(R)eturn to main menu\n\n";

        cout << "Enter the letter of your choice: ";

        string input;
        cin >> input;
        cout << "\n";
        input = cmpt::clean(input);
        cin.ignore(100, '\n');
        if (input == "n"){
            cout << "What is the name of the team: ";
            string search;
            getline(cin, search);
            printEntries(database.get_by_name(search));
        } else if (input == "g"){
            cout << "What game does your team play: ";
            string search;
            getline(cin, search);
            printEntries(database.get_by_game(cmpt::clean(search)));
        } else if (input == "l"){
            cout << "Where is your team located: ";
            string search;
            getline(cin, search);
            printEntries(database.get_by_location(search));
        } else if (input == "d"){
            cout << "What is the name of the team: ";
            string search;
            getline(cin, search);
            int i = stoi(cmpt::trim(search));
            printEntries(database.get_by_dateFounded(i));
        } else if (input == "r"){ 
            break;
        } else {
            cout << "Input was not valid! Try again!\n";
        }
        break;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Listing
///////////////////////////////////////////////////////////////////////////////
void listEntries(Database &database) {
    while (true){
        cout << "------Records List------\n";
        cout << "Would you like to list teams by: " << endl;
        cout << "(A)lphabetical Order\n"
                "(L)ocation\n"
                "(N)et Worth\n"
                "(Y)ear Founded\n"
                "--------------------------\n"
                "\n(G)o back to main menu\n\n";

        cout << "Enter the letter of your choice: ";

        string input;
        cin >> input;
        cout << endl;
        if (cmpt::clean(input) == "a"){
            cout << "Would you like your sort in reverse?" << endl;
            cout << "If \"Y\" isn't entered, your answer will be assumed as no: "; 
            string userInput;
            cin >> userInput;
            if(cmpt::clean(userInput) == "y") {
                database.sort_by_revname();
            } else {
                cout << "Here are the team names, in alphabetical order: " << endl;
                database.sort_by_name();
            }
            for(Team t : database.get_database()) {
                cout << t.get_full() << endl;
            }
            // database.replaceVector(temp);
            cout << endl;
        } else if (input == "l"){
            cout << "Would you like your sort in reverse?" << endl;
            cout << "If \"Y\" isn't entered, your answer will be assumed as no: "; 
            string userInput;
            cin >> userInput;
            if(cmpt::clean(userInput) == "y") {
                cout << "Here are the team names, with locations reverse sorted: " << endl;
                database.sort_by_revlocation();
            } else {
                cout << "Here are the team names, sorted by location: " << endl;
                database.sort_by_location();
            }
            for(Team t : database.get_database()) {
                cout << t.get_full() << " (" << t.get_location() << ")" << endl;
            }
            // database.replaceVector(temp);
            cout << endl;
        } else if (input == "n"){
            cout << "Would you like your sort in reverse?" << endl;
            cout << "If \"Y\" isn't entered, your answer will be assumed as no: "; 
            string userInput;
            cin >> userInput;
            if(cmpt::clean(userInput) == "y") {
                cout << "Here are the team names, with net worths reverse sorted: " << endl;
                database.sort_by_revnetWorth();
            } else {
                cout << "Here are the team names, sorted by net worth: " << endl;
                database.sort_by_netWorth();
            }
            for(Team t : database.get_database()) {
                cout << "$" << t.get_netWorth() << " million (" << t.get_full() << ")" << endl;
            }
            // database.replaceVector(temp);
            cout << endl;
        } else if (input == "y"){ 
            cout << "Would you like your sort in reverse?" << endl;
            cout << "If \"Y\" isn't entered, your answer will be assumed as no: "; 
            string userInput;
            cin >> userInput;
            if(cmpt::clean(userInput) == "y") {
                cout << "Here are the teams, sorted by newest-oldest: " << endl;
                database.sort_by_revyearFounded();
            } else {
                cout << "Here are the teams, sorted by oldest-newest: " << endl;
                database.sort_by_yearFounded();
            }
            for(Team t : database.get_database()) {
                cout << t.get_full() << " (Est. " << t.get_dateFounded() << ")" << endl;
            }
            // database.replaceVector(temp);
            cout << endl;
        } else if (input == "g"){ 
            break;
        }else {
            cout << "Input was not valid! Try again!\n";
        }
    }
}