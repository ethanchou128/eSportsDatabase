// divisions.cpp 

#include "divisions.h"
#include <string>
#include <vector>

using namespace std;

Divisions::Divisions()
{ }

///////////////////GETTERS////////////////////////
string Divisions::get_game() const { return game; }
vector<string> Divisions::get_roaster() const { return roaster; }
vector<string> Divisions::get_coach() const { return coach; }
string Divisions::get_win_loss() const { return win_loss; }
int Divisions::get_championships_won() const { return championships_won; }

///////////////////SETTERS////////////////////////
void Divisions::set_game(string s){
    game = s;
}

void Divisions::set_roaster(string s){
    roaster.push_back(s);
}

void Divisions::set_coach(string s){
    coach.push_back(s);
}

void Divisions::set_win_loss(string s){
    win_loss = s;
}

void Divisions::set_championships_won(int i){
    championships_won = i;
}