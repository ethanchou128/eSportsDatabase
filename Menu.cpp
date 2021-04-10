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

    WINDOW* win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
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


/////////////////////iosteam//////////////////////

void start_iostream(){

    cout << "Welcome to the eSport Team Database!\n";
    cout << "-----------------------------\n\n";

    Database database("database.txt");

    askMenu(database);

} 

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
        } else if (input == "q"){ 
            database.save_to_file("database.txt");
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
    temp.set_full(fullName);
    string shortName = getShortName();
    temp.set_short(shortName);
    vector<string> games = getDivList();
    temp.set_divList(games);
    // string president = getPresident();
    // temp.set_president(president);
    // string CEO = getCEO();
    // temp.set_ceo(CEO);
    // vector<string> sponsors = getPartners();
    // temp.set_partner(sponsors);
    string location = getLocation();
    temp.set_location(location);
    int yearFounded = getYearFounded();
    temp.set_dateFounded(yearFounded);
    double netWorth = getNetWorth();
    temp.set_netWorth(netWorth);
    database.add_team(temp);

    for(Team t : database.get_database()) {
        printEntries(t);
    }

}

void deleteEntry(Database &database) {
    if (database.get_database().empty()){
        cout << "There are no records in the database!\n\n";
        return;
    }
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

void findEntry(Database& database){
    if (database.get_database().empty()){
        cout << "There are no records in the database!\n\n";
        return;
    }

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

void printEntries(const vector<Team>& vT){
    if (vT.empty()){
        cout << "There are not records to print!\n";
        return;
    }
    for (Team t: vT){
        printEntries(t);
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
    // cout << t.get_president() << endl;
    // cout << t.get_ceo() << endl;
    // vector<string> p = t.get_partners();
    // for(string str : p) {
    //     cout << str << endl;
    // }
    cout << "Team location: " << t.get_location() << endl;
    cout << "Team founded date: " << t.get_dateFounded() << endl;
    cout << "Team Net Worth: " << t.get_netWorth() << endl;
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
    //getline(cin, userInput);
    cin >> userInput;
    //int hold = cmpt::trim(userInput);
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
    cout << "Please enter your team's region/country: ";
    string userInput;
    cin >> userInput;
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
