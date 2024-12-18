#pragma once
#include <SFML/Graphics.hpp>
#include "T.h"

using namespace std;
using namespace sf;

class Piece {
private:

public:

	virtual vector<Sprite> getSprites() = 0;
	virtual void rotate(string type, vector<vector<int>>& grid) = 0;
	virtual void fall() = 0;
	virtual void move(string type) = 0;
	virtual bool canMoveLeft(vector<vector<int>>& grid) = 0;
	virtual bool canMoveRight(vector<vector<int>>& grid) = 0;
	virtual bool canMoveDown(vector<vector<int>>& grid) = 0;
	virtual void updateGrid(vector<vector<int>>& grid) = 0;

};