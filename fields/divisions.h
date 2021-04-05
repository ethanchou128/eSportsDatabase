// divisions.h

#ifndef DIVISIONS_H
#define DIVISIONS_H

using namespace std;

#include <string>
#include <vector>

class Divisions {

    string game;
    vector<string> roaster;
    vector<string> coach;
    string win_loss;
    int championships_won;


public:

    Divisions();

    string get_game() const;
    vector<string> get_roaster() const;
    vector<string> get_coach() const;
    string get_win_loss() const;
    int get_championships_won() const;

    void set_game(string s);
    void set_roaster(string s);
    void set_coach(string s);
    void set_win_loss(string s);
    void set_championships_won(int i);

};

#endif