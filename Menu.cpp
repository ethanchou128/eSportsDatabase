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
#include <iomanip>
#include "tools.h"

using namespace std;

#define ctrl(x) (x & 0x1F)

//////////////////////////ncurses///////////////////////

void start(){

    initscr();
    noecho();
    raw();
    cbreak();
    curs_set(0);

    if (has_colors()){
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        assume_default_colors(COLOR_WHITE, COLOR_BLUE);
    }


    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* win = newwin(yMax*9/10, xMax*9/10, yMax/15, xMax/15);
    keypad(win, true);
    box(win,0,0);

    string options1[] = {"Add", "Return"};
    char options_trigger1[] = {'a', 'r'};

    string options2[] = {"Name", "Location", "Year", "Return"};
    char options_trigger2[] = {'n', 'l', 'y', 'r'};

    string options3[] = {"Name", "Game", "Location", "Date", "Return"};
    char options_trigger3[] = {'n', 'g', 'l', 'd', 'r'};

    string options4[] = {"Reversed", "Name", "Location", "Net Worth", "Year", "Return"};
    char options_trigger4[] = {ctrl('r'),'n', 'l', 'w', 'y', 'r'};

    string options5[] = {};
    char options_trigger5[] = {};
    
    Menu menus[5] = {
        Menu("Add", 'a', options1, options_trigger1, 2),
        Menu("Delete", 'd', options2, options_trigger2, 4), 
        Menu("Find", 'f', options3, options_trigger3, 5),
        Menu("List", 'l', options4, options_trigger4, 6),
        Menu("Quit", 'q', options5, options_trigger5, 0)
    };

    MenuOut menuout = MenuOut(win, menus, 5);
    menuout.reset();

    
    while (menuout.run == true){
        int ch = wgetch(win);
        menuout.handleTriggerMenu(ch);
        menuout.draw();
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
        cout << endl << "-------Menu-------\n";
        cout << "(A)dd a team.\n"
                "(F)ind a team.\n"
                "(D)elete a team.\n"
                "(L)ist teams.\n"
                "(Q)uit\n"
                "-------------------\n\n";

        cout << "Enter the letter of your choice: ";
        //input that determines which feature the user wants to use
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
//generic print teams function
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
    cout << "Team Net Worth (in millions): " << t.get_netWorth() << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Adding 
///////////////////////////////////////////////////////////////////////////////
void addEntry(Database& database) {
    Team temp;
    cout << "So you want to add a team? " << endl;
    cout << "If you would like to return to the main menu, please enter \"X\"";
    cout << "unless indicated otherwise." << endl;
    string fullName = getFullName(database);
    //checks to see if user just wants to quit; used below as well
    if(fullName != "x") { 
        temp.set_full(fullName);
    } else {
        return;
    }
    string shortName = getShortName();
    if(shortName != "x") {
        temp.set_short(shortName);
    } else {
        return;
    }
    vector<string> games = getDivList();
    if(games.size() != 0) {
        temp.set_divList(games);
    } else {
        return;
    }
    string location = getLocation();
    if(location != "x") {
        temp.set_location(location);
    } else {
        return;
    }
    int yearFounded = getYearFounded();
    if(yearFounded != -1) {
        temp.set_dateFounded(yearFounded);
    } else {
        return;
    }
    float netWorth = getNetWorth();
    if(netWorth != -1) {
        temp.set_netWorth(netWorth);
    } else {
        return;
    }
    database.add_team(temp);

    printEntries(database);

}
//retrieves full names from user
string getFullName(const Database& database) {
    cout << "Please enter team name: ";
    cin.ignore(100, '\n');
    string userInput;
    string s = "";
    getline(cin, userInput);
    for(char ch : userInput) {
        s += tolower(ch);
    }
    if(s == "x") {//check for user immediate exit
        return "x";
    }
    bool isValid = false;
    while(!isValid) {
        bool exists = false;
        for(Team t : database.get_database()) {
            if(t.get_full() == userInput) {
                exists = true;
            }
        }
        if(exists) { //checks to make sure duplicate isnt added
            cout << "That team already exists. Try again." << endl;
            getline(cin, userInput);
        } else if (userInput == "") {
            cout << "No input was detected. Try again." << endl;
            getline(cin, userInput);
        } else {
            isValid = true;
        }
    }
    return userInput;
};
//retrieve short name/abbreviation from user
string getShortName() {
    cout << "Please enter your team's short name: ";
    string userInput;
    cin >> userInput;
    string s = "";
    for(char ch : userInput) {
        s += tolower(ch);
    }
    if(s == "x") {
        return "x";
    }
    return userInput;
};
//function to retrieve vector of games the team plays
vector<string> getDivList() {
    cout << "Which games does your team participate in? Enter the exact game index number." << endl;
    cout << "When you have finished entering your list, please enter 0 to indicate so." << endl << endl;;
    cout << "**If you'd like to return to the main menu, please enter \"-1\".**" << endl << endl;
    vector<string> gamesPlayed;
    vector<string> eligibleGames;
    eligibleGames = {"Call of Duty", "CS:GO", "Fortnite", "League of Legends", "Overwatch", 
    "Rainbow Six Siege", "Rocket League", "Super Smash Bros.", "Valorant"};
    int userInput;
    cout << "Here are the eligible games: " << endl;
    for(int i = 0; i < eligibleGames.size(); i++) {
        cout << "(" << i+1 << ")" << " " << eligibleGames.at(i) << endl;
    }
    cin >> userInput;
    //if user enters something other than int, program crashes (successfully)
    if(cin.fail()) {
        cout << "You have not read instructions properly. Program is aborting now." << endl;
        exit(EXIT_FAILURE);
    }
    //statement to check if user wants to exit
    if(userInput == -1) {
        vector<string> empty = {};
        return empty;
    }
    while(userInput != 0) {
        bool isEligible = false;
        bool isEntered = false;
        for(int i = 0; i < eligibleGames.size(); i++) {
            if(i == userInput-1) {
                for(int j = 0; j < gamesPlayed.size(); j++) {
                    if(eligibleGames.at(i) == gamesPlayed.at(j)) {
                        isEntered = true;
                    }
                }
                if(!isEntered) {
                    cout << eligibleGames.at(i) << " has been added." << endl;
                    gamesPlayed.push_back(eligibleGames.at(i));
                    isEligible = true;
                } else {
                    cout << eligibleGames.at(i) << " has already been added.";
                    cout << " Please try again." << endl;
                    isEligible = true;
                }
            }
        }
        if(!isEligible) { //checks to see if option is plausible
            if(userInput == -1) {
                vector<string> empty = {};
                return empty;
            }
            cout << "You have entered an invalid or unpopular game. Please try again." << endl;
        }
        cin >> userInput;
    }
    //sorts gamesPlayed vector for convenience
    sort(gamesPlayed.begin(), gamesPlayed.end());
    cout << "Ok. Here are your team's games played: " << endl;
    //program repeats back what is inputted
    for(string s : gamesPlayed) {
        cout << s << endl;
    }
    return gamesPlayed;
};
//retrieves team headquarters
string getLocation() {
    cout << "Please enter your team's region/country: ";
    cin.ignore(100, '\n');
    string userInput;
    getline(cin, userInput);
    string s = "";
    for(char ch : userInput) {
        s += tolower(ch);
    }
    if(s == "x") {
        return "x";
    }
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
    cout << "If you'd like to return to the main menu, please enter \"-1\"." << endl;
    int userInput;
    cin >> userInput;
    //if int isnt entered, user is prompted again.
    while (cin.fail()){
        cout << "Sorry, that's an invalid entry. Please Try Again." << endl;
        cin >> userInput;
    }
    int num = userInput;
    return num;
};

float getNetWorth() {
    cout << "Please enter your team's net worth, in millions of dollars: ";
    string userInput;
    cin >> userInput;
    if(userInput == "-1") {
        return -1;
    }
    bool isNum = true;
    bool isDecimal = false;
    //loop to check if the input is a num (decimal or int)
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
    float num = stof(userInput);
    return num;

};

///////////////////////////////////////////////////////////////////////////////
// Deleting 
///////////////////////////////////////////////////////////////////////////////

const int MAXSETWIDTH = 30;
//sub-main menu
void deleteEntry(Database &database) {
    if (database.get_database().empty()){
        cout << "There are no records in the database!\n\n";
        return;
    }
    while (true){
        cout << "------Delete a Team by:------\n";
        cout << "(N)ame of team.\n"
                "(L)ocation of teams.\n"
                "(Y)ear teams were founded.\n"
                "--------------------------\n"
                "\n(R)eturn to main menu\n\n";

        cout << "Enter the letter of your choice: ";

        string input;
        cin >> input;
        cout << "\n";
        input = cmpt::clean(input);
        cin.ignore(100, '\n');
        if (input == "n"){
            deleteByName(database);
        } else if (input == "l"){
            deleteByLocation(database);
        } else if (input == "y"){
            deleteByYearFounded(database);
        } else if (input == "r"){ 
            break;
        } else {
            cout << "Input was not valid! Try again!\n";
        }
        break;
    }
    return;
}

void deleteByName(Database &database) {
    //first, teams within are listed
    cout << "Teams in database: \n";
    for(Team f : database.get_database()) {
        cout << f.get_full() << endl;
    }
    cout << "Please enter the team name that you want to delete; ";
    cout << "Enter the exact name as it appears: ";
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
        return;
    } else {
        vector<Team> temp = database.get_database();
        temp.erase(temp.begin() + indexCounter);
        database.replaceVector(temp);
        for(Team f : database.get_database()) {
            cout << f.get_full() << endl;
        }
    }
}

void deleteByLocation(Database &database) {
    //teams are listed (like deleteByName fct)
    cout << "Here are the teams, sorted by location: " << endl;
    database.sort_by_location();
    for(Team t : database.get_database()) {
        int length = t.get_full().length();
        cout << t.get_full() << setw(MAXSETWIDTH - length);
        cout << " -    " << t.get_location() << endl; 
    }
    cout << endl << "Which country's teams would you like to delete? ";
    string userInput;
    getline(cin, userInput);
    bool exists = false;
    int index = 0;
    int firstIndex = -1;
    int lastIndex = -2;
    vector<Team> temp = database.get_database();
    //loop that allows program to delete all entries with specified location
    for(Team t : temp) {
        if(t.get_location() == userInput) {
            if(!exists) {
                firstIndex = index;
                exists = true;
            }
        } else {
            if(firstIndex != -1) {
                lastIndex = index;
                break;
            }
        }
        index++;
    }
    temp.erase(temp.begin() + firstIndex, temp.begin() + lastIndex);
    database.replaceVector(temp);
    //reprint of remaining teams
    for(Team t : database.get_database()) {
        int length = t.get_full().length();
        cout << t.get_full() << setw(MAXSETWIDTH - length);
        cout << " -    " << t.get_location() << endl; 
    }
}

void deleteByYearFounded(Database &database) {
    cout << "Here are the teams, sorted by newest-oldest. " << endl;
    database.sort_by_revyearFounded();
    for(Team t : database.get_database()) {
        int length = t.get_full().length();
        cout << t.get_full() << setw(MAXSETWIDTH - length);
        cout << " - Est. " << t.get_dateFounded() << endl; 
    }
    cout << endl << "Which year's teams would you like to delete? ";
    int userInput;
    cin >> userInput;
    bool exists = false;
    int index = 0;
    int firstIndex = -1;
    int lastIndex = -2;
    vector<Team> temp = database.get_database();
    //same loop that checks so it can delete all entries with specified date
    for(Team t : temp) {
        if(t.get_dateFounded() == userInput) {
            if(!exists) {
                firstIndex = index;
                exists = true;
            }
        } else {
            if(firstIndex != -1) {
                lastIndex = index;
                break;
            }
        }
        index++;
    }
    temp.erase(temp.begin() + firstIndex, temp.begin() + lastIndex);
    database.replaceVector(temp);
    //reprints the revised list of teams in database
    for(Team t : database.get_database()) {
        int length = t.get_full().length();
        cout << t.get_full() << setw(MAXSETWIDTH - length);
        cout << " - Est. " << t.get_dateFounded() << endl; 
    }
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
        //after user chooses which field to sort by, they are
        //asked if they want to sort in reverse.
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
                int length = t.get_full().length();
                cout << t.get_full() << setw(MAXSETWIDTH - length);
                cout << " -    " << t.get_location() << endl; 
            }
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
                int length = t.get_full().length();
                cout << t.get_full() << setw(MAXSETWIDTH - length);
                cout << " -  $" << t.get_netWorth() << " million\n"; 
            }
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
                int length = t.get_full().length();
                cout << t.get_full() << setw(MAXSETWIDTH - length);
                cout << " -   Est. " << t.get_dateFounded() << endl; 
            }
            cout << endl;
        } else if (input == "g"){ 
            break;
        }else {
            cout << "Input was not valid! Try again!\n";
        }
    }
}