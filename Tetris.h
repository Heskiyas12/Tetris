#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include "RNG.h"
#include <vector>
#include <unordered_map>
#include "T.h"
#include "Piece.h"
#include "J.h"
#include "Z.h"
#include "Square.h"
#include "S.h"
#include "L.h"
#include "Line.h"
#include <queue>
#include <fstream>

using namespace std;
using namespace sf;

class Game
{
private:
    RenderWindow* window;
    int level;

    void initializeGrid(vector<vector<int>>& grid);
    void displayGrid(vector<vector<int>>& grid);
    void drawGameGrid(RenderWindow& window, vector<vector<int>>& grid, Texture& gameArt);
    void shiftLinesDown(vector<vector<int>>& grid, int clearedRow);
    void clearFullLines(vector<vector<int>>& grid, int level, int& score, int& totalLinesCleared);
    string integerToString(int score, int length);
    bool checkGameOver(vector<vector<int>>& grid);
    void readTopScores(ifstream& file, vector<int>& scores);
    void updateTopScore(vector<int>& scores, int score);

public:
    Game(int level, RenderWindow& window);
    void run();
};

#endif 
