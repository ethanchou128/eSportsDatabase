// design.cpp 

#include "Design.h"
#include "Database.h"
#include "Team.h"
#include "tools.h"
#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <cstdio>   
#include <algorithm>
#include <vector>

#define ctrl(x) (x & 0x1F)

using namespace std;
//------------------------------------------------------
/**
 * Menu Class
 */
//------------------------------------------------------
Menu::Menu(string text, char trigger, string* options, char* options_trigger, int num_options)
{
    this->text = text;
    this->trigger = trigger;
    this->options = options;
    this->options_trigger = options_trigger;
    this->num_options = num_options;
    this->selected_option = -1;
    this->selected = false;
    this->reversed = false;
}




//------------------------------------------------------
/**
 * Used to change the selected sub-menu
 */
//------------------------------------------------------
void Menu::selectNextOption(){
    selected_option++;
    if (selected_option >= num_options){
        selected_option = 0;
    }
}

void Menu::selectPreviousOption(){
    selected_option--;
    if (selected_option < 0){
        selected_option = num_options - 1;
    }
}


//------------------------------------------------------
/**
 * MenuOut Class
 */
//------------------------------------------------------
MenuOut::MenuOut(WINDOW* win, Menu* menus, int num_menus){
    this->win = win;
    this->menus = menus;
    this->num_menus = num_menus;
    this->selected_menu = -1;
    this->selected = false;
    this->run = true;
    Database database("database.txt");
    this->database = database;
    database.sort_by_name();

    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);
    optionwin = newwin(yMax, xMax, yBeg, xBeg);
    keypad(optionwin, true);
    wrefresh(win);

    int start = getmaxx(win) / (num_menus + 1);
    int current_pos;

    for (int i = 0; i < num_menus; i++){
        int start_options = getmaxx(win) / (this->menus[i].num_options + 1);
        current_pos = start + (start * i) - this->menus[i].text.length()/2;
        this->menus[i].start_x_menu = current_pos;
        this->menus[i].start_x_options = start_options;
    }
}


//------------------------------------------------------
/**
 * Main drawing / output code
 * It is responsible to print the menu bar and information
 * about the menu and its options
 */
//------------------------------------------------------
void MenuOut::reset(){
    werase(win);
    box(win, 0, 0);
    wrefresh(win);
    for (int i = 0; i < num_menus; i++){
        drawMenu(menus[i], selected_menu == i);
    }
    
    print_centered(win, 2, "Welcome to Justin and Ethan's Database of Esports Teams");
    print_centered(win, 3, "Press the letter that corresponds to the action to select it");
    print_centered(win, 4, "Alternatively you can use the arrow keys go through the menu");
    print_centered(win, 5, "Press Enter once you have selected an option");

    wrefresh(win);
}

void MenuOut::menureset(Menu& menu){
    box(optionwin, 0, 0);
    int start = menu.start_x_options;
    int current_pos;
    for (int i = 0; i < menu.num_options; i++){
        if (menu.selected_option == i){
            if ((selected_menu == 0 || selected_menu == 2) && menu.selected){
                wattron(optionwin, A_BLINK);
            } 
            wattron(optionwin, A_STANDOUT);
            
        }
        if ((menu.options[i] == "Reversed") && menu.reversed){
            init_pair(2, COLOR_CYAN, COLOR_WHITE);
            wattron(optionwin, COLOR_PAIR(2));
        }
        current_pos = start + (start * i) - menu.options[i].length() / 2;
        mvwprintw(optionwin, 0, current_pos, menu.options[i].c_str());
        wattroff(optionwin, A_STANDOUT);
        wattroff(optionwin, COLOR_PAIR(2));
        wattroff(optionwin, A_BLINK);
        wrefresh(optionwin);
    }
}

void MenuOut::draw(){
    if (!selected){
        for (int i = 0; i < num_menus; i++){
            drawMenu(menus[i], selected_menu == i);
        }
    } else if (selected && selected_menu == num_menus - 1){
        database.save_to_file("database.txt");
        run = false;
    } else {
        int ch;
        drawMenuOptions(menus[selected_menu]);
        wrefresh(win);
        while(selected && (ch = wgetch(optionwin))){

            handleTriggerOptions(menus[selected_menu], ch);
            if (menus[selected_menu].selected){
                if (menus[selected_menu].selected_option == menus[selected_menu].num_options - 1 
                    && menus[selected_menu].selected == true){
                    menus[selected_menu].selected = false;
                    menus[selected_menu].selected_option = -1;
                    selected = false;
                }
            }

            if(menus[selected_menu].selected || menus[selected_menu].selected_option == -1){
                wclear(optionwin);
                wrefresh(optionwin);
            }
            drawMenuOptions(menus[selected_menu]);
            menus[selected_menu].selected = false;
        }
        menus[selected_menu].reversed = false;
        wclear(optionwin);
        wrefresh(optionwin);
        reset();
        selected = false;
    }
}

void MenuOut::drawMenu(Menu menu, bool is_selected){
    int start_x = menu.start_x_menu;
    string text = menu.text;
    if (is_selected){
        wattron(win, A_STANDOUT);
    }
    mvwprintw(win, 0, start_x, text.c_str());
    wattroff(win, A_STANDOUT);
    wrefresh(win);

}

void MenuOut::drawMenuOptions(Menu& menu){
    box(optionwin, 0, 0);
    int start = menu.start_x_options;
    int current_pos;
    for (int i = 0; i < menu.num_options; i++){
        if (menu.selected_option == i){
            if ((selected_menu == 0 || selected_menu == 2) && menu.selected){
                wattron(optionwin, A_BLINK);
            } 
            wattron(optionwin, A_STANDOUT);
            
        }
        if ((menu.options[i] == "Reversed") && menu.reversed){
            init_pair(2, COLOR_CYAN, COLOR_WHITE);
            wattron(optionwin, COLOR_PAIR(2));
        }
        current_pos = start + (start * i) - menu.options[i].length() / 2;
        mvwprintw(optionwin, 0, current_pos, menu.options[i].c_str());
        wattroff(optionwin, A_STANDOUT);
        wattroff(optionwin, COLOR_PAIR(2));
        wattroff(optionwin, A_BLINK);
        wrefresh(optionwin);
    }
    //this is the switch that determines which feature the user wants to access
    if (selected){ 
        switch (selected_menu){
            case 0:
                addEntry(menus[selected_menu]);
                // adding
                break;
            case 1:
                // deleting
                deleteEntry(menus[selected_menu]);
                break;
            case 2:
                // finding
                findMenu(menus[selected_menu]);
                break;
            case 3:
                // listing
                listMenu(menus[selected_menu]);
                break;
            default:
                break;
        }
    }
    
}

//------------------------------------------------------
/**
 * Handles user inputs
 * Is used to change selected menu as well as select the menu
 * Removed ctrl+c break!!!
 * Use ctrl+w to "exit", except for main menu
 */
//------------------------------------------------------
void MenuOut::handleTriggerMenu(int trigger){
    if (trigger >= 10 && trigger <= 13){
        if (selected_menu != -1){
            this->selected = true;
        }
        return;
    }
    switch (trigger){
        case KEY_RIGHT:
            selectNextMenu();
            break;
        case KEY_LEFT:
            selectPreviousMenu();
            break;
        default:
            selected_menu = -1;
            break;
    }

    for (int i = 0; i < num_menus; i++){
        if (trigger == this->menus[i].trigger){
            selected_menu = i;
        }
    }
}

void MenuOut::handleTriggerOptions(Menu& menu, int trigger){
    if (trigger == ctrl('w')){
        selected = false;
    }

    if (trigger == ctrl('r') && this->selected_menu == 3){
        if (menu.reversed == true){
            menu.reversed = false;
        } else {
            menu.reversed = true;
        }
        if (menu.selected_option != menu.num_options - 1){
            menu.selected = true;
        }
        return;
    }
    
    if (trigger >= 10 && trigger <= 13){
        if (menu.options[0] == "Reversed" && menu.selected_option == 0){
            if (menu.reversed == true){
                menu.reversed = false;
            } else {
                menu.reversed = true;
            }
        }
        menu.selected = true;
        return;
    }

    switch (trigger){
        case KEY_RIGHT:
            menu.selectNextOption();
            break;
        case KEY_LEFT:
            menu.selectPreviousOption();
            break;
        default:
            menu.selected_option = -1;
            break;
    }

    for (int i = 0; i < menu.num_options; i++){
        if (trigger == menu.options_trigger[i]){
            menu.selected_option = i;
        }
    }
}


//------------------------------------------------------
/**
 * Used to change the selected main menu
 */
//------------------------------------------------------
void MenuOut::selectNextMenu(){
    selected_menu++;
    if (selected_menu >= num_menus){
        selected_menu = 0;
    }
}

void MenuOut::selectPreviousMenu(){
    selected_menu--;
    if (selected_menu < 0){
        selected_menu = num_menus - 1;
    }
}


//------------------------------------------------------
/**
 * Printing functions
 * Use to properly format text in the window
 */
//------------------------------------------------------
void MenuOut::print_centered(WINDOW* win, int start_row, string text){
    int center = getmaxx(win) / 2;
    int half = text.length() / 2;
    int adj = center - half;

    mvwprintw(win, start_row, adj, text.c_str());
}

void MenuOut::printEntries(WINDOW* win, Database& database){
    database.sort_by_name();
    printEntries(win, database.get_database());
}

void MenuOut::printEntries(WINDOW* win, const vector<Team>& vT){
    werase(win);
    box(win, 0, 0);
    wrefresh(win);

    if (vT.empty()){
        print_centered(win, 2, "There are no records to print!");
        return;
    }
    int i = 1;
    
    for (Team t: vT){
        string txt = "Team " + i + ':';
        print_centered(win, 2, txt);
        print_centered(win, 3, "------");
        printEntries(win, t);
        i++;
    }
}

void MenuOut::printEntries(WINDOW* win, const Team& t) {
    print_centered(win, 4, "Team Full Name: " + t.get_full());
    print_centered(win, 5, "Team Short Name: " + t.get_short());
    print_centered(win, 6, "Games Played by Team:");
    vector<string> v = t.get_divList();
    int final;
    for (int i = 0; i < v.size(); i++){
        final = 7 + i;
        print_centered(win, final, v.at(i));
    }
    print_centered(win, final + 1, "Team location: " + t.get_location());
    print_centered(win, final + 2, "Team founded date: " + to_string(t.get_dateFounded()));

    string text = "Team Net Worth: %.2f million";
    int center = getmaxx(win) / 2;
    int half = text.length() / 2;
    int adj = center - half;
    mvwprintw(win, final + 3, adj, "Team Net Worth: %.2f million", t.get_netWorth());
}


//------------------------------------------------------
/**
 * Calls when user wants to add record
 */
//------------------------------------------------------
void MenuOut::addEntry(Menu& menu) {
    //minor menu to indicate instructions, etc.
    if (menu.selected_option == -1){
        string msg = "Here you can add a team to the database";
        print_centered(optionwin, 2, msg.c_str());
        msg = "You can exit this menu by selecting 'return' or by pressing 'ctrl+w'";
        print_centered(optionwin, 3, msg.c_str());
    } 
    //user add entry option
    else if (menu.selected && menu.selected_option > -1){
        
        curs_set(1);
        int i = 0;
        int row = 2;
        int center = getmaxx(optionwin) / 2;
        Team temp;
        while(menu.selected){
            string str;
            switch (i){
                case 0:{
                    print_centered(optionwin, row, "What's the name of your team?");
                    
                    bool running = true;
                    while(running) {
                        bool exists = false;
                        int center = getmaxx(optionwin) / 2;
                        wmove(optionwin, 3, center - 10);
                        str = getaddinput(menu);
                        for(Team t : database.get_database()) {
                            string full = t.get_full();
                            full = cmpt::to_lower(full);
                            if(cmpt::to_lower(str) == full) {
                                exists = true;
                                break;
                            }

                        }
                        if(exists) {
                            print_centered(optionwin, row, "                             ");
                            print_centered(optionwin, row, "Team already exists. Try again.");
                            print_centered(optionwin, row+1, "                           ");
                        } else {
                            temp.set_full(str);
                            running = false;
                        }
                    }
                    break;
                } 
                case 1:{
                    print_centered(optionwin, row, "What's your team's short name?");
                    wmove(optionwin, row + 1, center - 10);
                    str = getaddinput(menu);
                    temp.set_short(str);
                    break;
                }
                case 2:{
                    print_centered(optionwin, row, "Which games does your team participate in? ");
                    row++;
                    print_centered(optionwin, row, "Type the number of the game to add! ");
                    row++;
                    print_centered(optionwin, row, "Enter \"0\" when finished.");
                    vector<string> gamesPlayed;
                    //vector that stores valid games; if we held all games this folder is too big
                    vector<string> eligibleGames;
                    eligibleGames = {"Call of Duty", "CS:GO", "Fortnite", "League of Legends", 
                    "Overwatch", "Rainbow Six Siege", 
                    "Rocket League", "Super Smash Bros.", "Valorant"};
                    for(int i = 0; i < eligibleGames.size(); i++) {
                        mvwprintw(optionwin, row + i - 2, 0, to_string(i + 1).c_str());
                        mvwprintw(optionwin, row + i - 2, 2, eligibleGames.at(i).c_str());
                    }
                    wmove(optionwin, row + 1, center - 10);
                    str = getaddinput(menu);
                    int game = -1;
                    bool between = true;
                    while(game != 0){
                        try {
                            game = stoi(str);
                            if (game == 0){
                                break;
                            }
                            
                        } catch (...){
                            
                        }
                        if (game > 0 && game <= eligibleGames.size()){
                            gamesPlayed.push_back(eligibleGames.at(game - 1));
                            for(int i = 0; i < eligibleGames.size(); i++) {
                                if (i == game - 1){
                                    mvwprintw(optionwin, row + i - 2, 1, "+");
                                }
                            }
                        }
                        if (between){
                            mvwprintw(optionwin, row + 1, center - 10, "                   ");
                            wmove(optionwin, row + 1, center - 10);
                            wrefresh(optionwin);
                            str = getaddinput(menu);
                            if (menu.selected == false){
                                break;
                            }
                        }
                    }
                    sort(gamesPlayed.begin(), gamesPlayed.end());
                    temp.set_divList(gamesPlayed);
                    break;
                } 
                case 3:{
                    print_centered(optionwin, row, "Where is your team headquartered?");
                    wmove(optionwin, row + 1, center - 10);
                    str = getaddinput(menu);
                    temp.set_location(str);
                    break;
                } 
                case 4:{
                    print_centered(optionwin, row, "What year was your team founded?");
                    wmove(optionwin, row + 1, center - 10);
                    str = getaddinput(menu);
                    if (menu.selected == false){
                        break;
                    }
                    int year = -1;
                    bool between = true;
                    while(year < 0 || between){
                        try {
                            year = stoi(str);
                            if (year > 1950 && year < 2022){
                                break;
                            }
                            
                        } catch (...){
                            
                        }
                        if (year > 1950 && year < 2022){
                            between = false;
                        }
                        if (between){
                            mvwprintw(optionwin, row + 1, center - 10, "                   ");
                            wmove(optionwin, row + 1, center - 10);
                            wrefresh(optionwin);
                            str = getaddinput(menu);
                            if (menu.selected == false){
                                break;
                            }
                        }
                    }
                    temp.set_dateFounded(year);
                    break;
                } 
                case 5:{
                    print_centered(optionwin, row, "What's your team's net worth, in millions of dollars?");
                    wmove(optionwin, row + 1, center - 10);
                    str = getaddinput(menu);
                    if (menu.selected == false){
                        break;
                    }
                    float worth = -1;
                    while(worth < 0){
                        try {
                            
                            worth = stof(str);
                            
                        } catch (...){
                            mvwprintw(optionwin, row + 1, center - 10, "                   ");
                            wmove(optionwin, row + 1, center - 10);
                            wrefresh(optionwin);
                            str = getaddinput(menu);
                            if (menu.selected == false){
                                break;
                            }   
                        }
                    }
                    temp.set_netWorth(worth);

                    print_centered(optionwin, row+1, "Team added!");
                    database.add_team(temp);
                    break;
                } 
                default:
                    menu.selected = false;
            }
            i++;
            row += 2;
        }

        if (i == 6){
            print_centered(optionwin, row+2, "Quitted!");
        }
        
        noecho();
        curs_set(0);
        menu.selected = false;
        menureset(menu);
    }

}

string MenuOut::getaddinput(Menu& menu){
    string input;
    int ch;
    while ((ch = wgetch(optionwin))){
        if ( ch == ERR || ch == '\n'){
            return input;
        } else if (ch == ctrl('w')){
            menu.selected = false;
            menu.selected_option = -1;
            return "";
        } else if (ch == KEY_BACKSPACE){
            int curx = getcurx(optionwin);
            int cury = getcury(optionwin);
            if (!input.empty()){
                input.pop_back();
                mvwprintw(optionwin, cury, curx - 1, " ");
                wmove(optionwin, cury, curx - 1);
            } else {
                mvwprintw(optionwin, cury, curx, " ");
                wmove(optionwin, cury, curx);
            }
        } else {
            char hold = ch;
            int curx = getcurx(optionwin);
            int cury = getcury(optionwin);
            mvwprintw(optionwin, cury, curx, "%c", hold);
            input.append(1, hold);
        }
    }
    return input;
}

//------------------------------------------------------
/**
 * Calls when user wants to delete record
 */
//------------------------------------------------------
void MenuOut::deleteEntry(Menu &menu) {
    if (!menu.selected && menu.selected_option == -1){
        string msg = "Here you can delete records based on chosen field";
        print_centered(optionwin, 2, msg.c_str());
        msg = "You can exit this menu by selecting 'return' or by pressing 'ctrl+w'";
        print_centered(optionwin, 3, msg.c_str());
        msg = "work in progress";
        print_centered(optionwin, 4, msg.c_str());
        
    } else if (menu.selected){
        curs_set(1);
        switch (menu.selected_option){
            case 0: {

                print_centered(optionwin, 3, "What is the name of the team?");
                print_centered(optionwin, 5, "Here are the options:");
                bool running = true;
                vector<Team> temp = database.get_database();
                int initRowIndex = 6;
                for(int i = 0; i < temp.size(); i++) { //loop that prints options
            //this loop is featured in the other delete entry sections, modified accordingly
                    print_centered(optionwin, initRowIndex, temp.at(i).get_full());
                    initRowIndex++;
                }
                while(running) {
                    int center = getmaxx(optionwin) / 2;
                    wmove(optionwin, 4, center - 5);
                    string str = getaddinput(menu);
                    int vectorIndex = 0;
                    if (str.empty()){
                        running = false;
                        break;
                    }
                    for(Team t : temp) {
                        string full = t.get_full();
                        full = cmpt::to_lower(full);
                        string nick = t.get_short();
                        nick = cmpt::to_lower(nick);
                        str = cmpt::to_lower(str);
                        if(full.find(str) != string::npos || nick.find(str) != string::npos){
                            temp.erase(temp.begin() + vectorIndex);
                            running = false;
                        }
                        vectorIndex++;
                    }
                    if(running) {
                        print_centered(optionwin, 3, "That team doesn't exist. Try again.");
                        mvwprintw(optionwin, 4, center - 10, "                   ");
                    }
                }
                menureset(menu);
                int i = 5;
                mvwprintw(optionwin, 4, i - 1, "                   ");
                mvwprintw(optionwin, 4, i, "                   ");
                print_centered(optionwin, i-1, "Teams remaining:");
                for (Team t : temp){
                    print_centered(optionwin, i, "                            ");
                    print_centered(optionwin, i, t.get_full().c_str());
                    i++;
                }
                print_centered(optionwin, i, "                            ");
                database.replaceVector(temp);
                break;
            }
            case 1: {
                database.sort_by_location();
                print_centered(optionwin, 3, "Which region's teams would you like to omit?");
                print_centered(optionwin, 5, "Here are the options:");
                bool running = true;
                vector<Team> temp = database.get_database();
                int initRowIndex = 6;
                for(int i = 0; i < temp.size(); i++) {
                    string format = database.get_database().at(i).get_full() + " -  "; 
                    format.append(database.get_database().at(i).get_location());
                    print_centered(optionwin, initRowIndex, format);
                    initRowIndex++;
                }
                int vectorIndex = 0;
                int firstIndex = -1;
                int lastIndex = -2;
                bool exists = false;
                while(running) {
                    int center = getmaxx(optionwin) / 2;
                    wmove(optionwin, 4, center - 10);
                    string str = getaddinput(menu);
                    if (str.empty()){
                        running = false;
                        break;
                    }
                    for(Team t : temp) {
                        if(cmpt::to_lower(str) == cmpt::to_lower(t.get_location())) {
                            if(!exists) {
                                firstIndex = vectorIndex;
                                exists = true;
                            }
                        } else {
                            if(firstIndex != -1) {
                                lastIndex = vectorIndex;
                                break;
                            }
                        }
                        vectorIndex++;
                    }
                    if(!exists) {
                        print_centered(optionwin, 3, "This region does not exist. Please try again.");
                        mvwprintw(optionwin, 4, center - 10, "                      ");
                    } else {
                        temp.erase(temp.begin() + firstIndex, temp.begin() + lastIndex);
                        running = false;
                    }
                }
                menureset(menu);
                int rowIndex = 5;
                print_centered(optionwin, rowIndex-1, "                          ");
                print_centered(optionwin, rowIndex-1, "Teams remaining:");
                for(int index = 0; index < database.get_database().size(); index++) {
                    string blank = "                                     ";
                    print_centered(optionwin, rowIndex+index, blank);
                }
                database.replaceVector(temp);
                for (int i = 0; i < database.get_size(); i++){
                    string format = database.get_database().at(i).get_full() + " -  "; 
                    format.append(database.get_database().at(i).get_location());
                    print_centered(optionwin, rowIndex + i, format);
                    
                }
                
                break;
            }
            case 2: {
                database.sort_by_yearFounded();
                print_centered(optionwin, 3, "Which year's teams would you like to omit?");
                print_centered(optionwin, 5, "Here are the options:");
                bool exists = false;
                int year = -1;
                int vectorIndex = 0;
                int firstIndex = -1;
                int lastIndex = -2;
                bool between = true;
                vector<Team> temp = database.get_database();
                int initRowIndex = 6;
                for(int i = 0; i < temp.size(); i++) {
                    string format = database.get_database().at(i).get_full() + " -   Est. "; 
                    format.append(to_string(database.get_database().at(i).get_dateFounded()));
                    print_centered(optionwin, initRowIndex, format);
                    initRowIndex++;
                }
                while(between || year < 0 || !exists) {
                    int center = getmaxx(optionwin) / 2;
                    wmove(optionwin, 4, center - 10);
                    string str = getaddinput(menu);
                    if (str.empty()){
                        break;
                    }
                    try {
                        year = stoi(str);
                    } catch (...) {

                    }
                    if(year > 1950 && year < 2022) {
                        between = false;
                    }
                    if(between){
                        mvwprintw(optionwin, 3, center - 10, "                                  ");
                        print_centered(optionwin, 3, "That's an invalid option. Please try again.");
                        mvwprintw(optionwin, 4, center - 10, "                              ");
                    } else {
                        for(Team t : temp) {
                            if(year == t.get_dateFounded()) {
                                if(!exists) {
                                    firstIndex = vectorIndex;
                                    exists = true;
                                }
                            } else {
                                if(firstIndex != -1) {
                                    lastIndex = vectorIndex;
                                    break;
                                }
                            }
                            vectorIndex++;
                        }
                        if(!exists) {
                            mvwprintw(optionwin, 3, center - 10, "                              ");
                            print_centered(optionwin, 3, "This year is invalid. " 
                            "Please try again.");
                            mvwprintw(optionwin, 4, center - 10, "                              ");
                            wmove(optionwin, 4, center - 10);

                        } else {
                            temp.erase(temp.begin() + firstIndex, temp.begin() + lastIndex);
                        }    
                    }
                    
                }
                print_centered(optionwin, 3, "                                               ");
                int i = 5;
                //print_centered(optionwin, i-1, "Teams remaining:");
                for(int index = 0; index < database.get_database().size(); index++) {
                    string blank = "                                     ";
                    print_centered(optionwin, i+index, blank);
                }
                for (Team t : temp){
                    int rowIndex = 5;
                    print_centered(optionwin, rowIndex-1, "   Teams remaining:   ");
                    database.replaceVector(temp);
                    for (int i = 0; i < database.get_size(); i++){
                        string format = database.get_database().at(i).get_full() + " -   Est. "; 
                        format.append(to_string(database.get_database().at(i).get_dateFounded()));
                        print_centered(optionwin, rowIndex + i, format);
                    }
                    break;
                }
                database.replaceVector(temp);
                break;
            }
        }
        noecho();
        curs_set(0);
        menu.selected = false;
        menureset(menu);
    }
}



//------------------------------------------------------
/**
 * Calls when user wants to find record
 */
//------------------------------------------------------
void MenuOut::findMenu(Menu& menu){
    if (!menu.selected && menu.selected_option == -1){
        string msg = "Here you can search records based on chosen field";
        print_centered(optionwin, 2, msg.c_str());
        msg = "You can exit this menu by selecting 'return' or pressing 'ctrl+w'";
        print_centered(optionwin, 3, msg.c_str());
        msg = "Pressing 'ctrl+w' stop the action";
        print_centered(optionwin, 4, msg.c_str());
        
    } else if (menu.selected){
        curs_set(1);
        switch (menu.selected_option){
            case 0: {
                print_centered(optionwin, 3, "What is the name of the team?");
                int center = getmaxx(optionwin) / 2;
                wmove(optionwin, 4, center - 10);
                string str = getaddinput(menu);
                print_centered(optionwin, 5, "Teams:");
                print_centered(optionwin, 6, "--------");
                int i = 7;
                for (Team t : database.get_by_name(cmpt::to_lower(str))){
                    vector<Team> temp = database.get_by_name(str);
                    print_centered(optionwin, 15, to_string(temp.size()));
                    if(temp.size() == 0) {
                        string alert = "No teams were found with that name.";
                        print_centered(optionwin, i, alert);
                    } else {
                        for (Team t : temp){
                            if(str == "") {
                                string alert = "Empty string. All teams will be printed.";
                                print_centered(optionwin, i, alert);
                                break;
                                //if the user just enters a space, the program will
                                //print all team name in the database.
                            } else {
                                print_centered(optionwin, i, t.get_full().c_str());
                                i++;
                            }
                        }
                    }
                }
                break;
            }
            case 1: {
                print_centered(optionwin, 3, "What are the games your team plays?");

                int center = getmaxx(optionwin) / 2;
                wmove(optionwin, 4, center - 10);
                string str = getaddinput(menu);
                print_centered(optionwin, 5, "Teams:");
                print_centered(optionwin, 6, "--------");
                int i = 7;
                for (Team t : database.get_by_game(cmpt::to_lower(str))){
                    vector<Team> temp = database.get_by_game(str);
                    if(temp.size() == 0) {
                        string alert = "No games exist in the database.";
                        print_centered(optionwin, i, alert);
                    } else {
                        for (Team t : temp){
                            if(str == "") {
                                string alert = "Empty string. All teams will be printed.";
                                print_centered(optionwin, i, alert);
                                break;
                                //if the user just enters a space, the program will
                                //print all team name in the database.
                            } else {
                                print_centered(optionwin, i, t.get_full().c_str());
                                i++;
                            }
                        }
                    }
                }
                break;
            }
            case 2: {
                print_centered(optionwin, 3, "Where is your team located (country)?");
                int center = getmaxx(optionwin) / 2;
                wmove(optionwin, 4, center - 10);
                string str = getaddinput(menu);
                print_centered(optionwin, 5, "Teams:");
                print_centered(optionwin, 6, "--------");
                int i = 7;
                for (Team t : database.get_by_location(cmpt::to_lower(str))){
                    vector<Team> temp = database.get_by_location(str);
                    if(temp.size() == 0) {
                        string alert = "No teams were found with that region.";
                        print_centered(optionwin, i, alert);
                    } else {
                        for (Team t : temp){
                            if(str == "") {
                                string alert = "Empty string. All teams will be printed.";
                                print_centered(optionwin, i, alert);
                                break;
                                //if the user just enters a space, the program will
                                //print all team name in the database.
                            } else {
                                print_centered(optionwin, i, t.get_full().c_str());
                                i++;
                            }
                        }
                    }
                }
                break;
            }
            case 3: {
                print_centered(optionwin, 3, "When was your team founded?");

                int center = getmaxx(optionwin) / 2;
                bool isNum = true;
                bool running = true;
                //string str;
                while(running) {
                    wmove(optionwin, 4, center - 10);
                    string str = getaddinput(menu);
                    str = cmpt::clean(str);
                    for(int i = 0; i < str.length(); i++) {
                        if(!isdigit(str.at(i))) {
                            isNum = false;
                            break;
                        }
                    }
                    if(isNum && str != "") {
                        print_centered(optionwin, 5, "Teams:");
                        print_centered(optionwin, 6, "--------");
                        int i = 7;
                        vector<Team> temp = database.get_by_location(str);
                        if(temp.size() == 0) {
                            string alert = "No teams were found with that date.";
                            print_centered(optionwin, i, alert);
                        } else {
                            for (Team t : temp){
                                if(str == "") {
                                    string alert = "Empty string. All teams will be printed.";
                                    print_centered(optionwin, i, alert);
                                    break;
                                    //if the user just enters a space, the program will
                                    //print all team name in the database.
                                } else {
                                    print_centered(optionwin, i, t.get_full().c_str());
                                    i++;
                                }
                            }
                        }
                        running = false;
                    } else {
                        print_centered(optionwin, 3, "That number is invalid. Try again.");
                    }
                }
                break;
            }

        }
    }
    noecho();
    curs_set(0);
    menu.selected = false;
    menureset(menu);
}





//------------------------------------------------------
/**
 * Calls when user wants to list record
 */
//------------------------------------------------------
void MenuOut::listMenu(Menu& menu){
    if (!menu.selected && menu.selected_option == -1){
        string msg = "Here you can list all the records";
        print_centered(optionwin, 2, msg.c_str());
        msg = "You can exit this menu by selecting 'return' or by pressing 'ctrl+w'";
        print_centered(optionwin, 3, msg.c_str());
        msg = "The 'Reversed' option is a toggle.";
        print_centered(optionwin, 4, msg.c_str());
        msg = "When it is highlighted, options will be sored in reverse!";
        print_centered(optionwin, 5, msg.c_str());
        msg = "Pressing 'ctrl+r' will toggle reversed without having to select it!";
        print_centered(optionwin, 6, msg.c_str());
    } else if (menu.selected){
        switch (menu.selected_option){
            case 0: {
                string msg = "Reversed has been toggled!";
                print_centered(optionwin, 3, msg.c_str());
                msg = "Selecting options will be sorted accordingly";
                print_centered(optionwin, 4, msg.c_str());
                break;
            }
            case 1: {
                int start = 2;
                if (menu.reversed){
                    database.sort_by_revname();
                } else {
                    database.sort_by_name();
                }
                for (int i = 0; i < database.get_size(); i++){
                    print_centered(optionwin, start + i, 
                    database.get_database().at(i).get_full().c_str());
                }
                break;
            }
            case 2:{
                int start = 2;
                if (menu.reversed){
                    database.sort_by_revlocation();
                } else {
                    database.sort_by_location();
                }

                for (int i = 0; i < database.get_size(); i++){
                    string format = database.get_database().at(i).get_full() + " -  "; 
                    format.append(database.get_database().at(i).get_location());
                    print_centered(optionwin, start + i, format);
                    
                }
                break;
            }
            case 3:{
                int start = 2;
                if (menu.reversed){
                    database.sort_by_revnetWorth();
                } else {
                    database.sort_by_netWorth();
                }

                for (int i = 0; i < database.get_size(); i++){
                    string format = database.get_database().at(i).get_full() + " -  $"; 
                    format.append("%.2f");
                    format.append(" million");
                    int center = getmaxx(optionwin) / 2;
                    int half = format.length() / 2;
                    int adj = center - half;
                    mvwprintw(optionwin, start + i, adj, format.c_str(), 
                    database.get_database().at(i).get_netWorth());
                    
                }
                break;
            }
            case 4:{
                int start = 2;
                if (menu.reversed){
                    database.sort_by_revyearFounded();
                } else {
                    database.sort_by_yearFounded();
                }

                for (int i = 0; i < database.get_size(); i++){
                    string format = database.get_database().at(i).get_full() + " -   Est. "; 
                    format.append(to_string(database.get_database().at(i).get_dateFounded()));
                    print_centered(optionwin, start + i, format);
                    
                }
                break;
            }
        }
    }
    database.sort_by_name();
}
