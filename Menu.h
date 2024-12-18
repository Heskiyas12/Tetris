#pragma once
#include <SFML/Graphics.hpp>
#include "Tetris.h"

using namespace std;
using namespace sf;

class Menu
{
private:
    RenderWindow* window;
    void getHighScores(vector<int>& scores, ifstream& file);
    string numAsString(int score, int length);

public:

    Menu(RenderWindow& window);
    void displayMenu();

};