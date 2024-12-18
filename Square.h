
#include "Piece.h"

class Square : public Piece {

private:

    Sprite block1;
    Sprite block2;
    Sprite block3;
    Sprite block4;
    vector<Sprite> sprites;
    float scaleFactor;
    int state;
    Clock clock;

    // Function to update the rotation state of the piece
    void updateState(int& state, string type)
    {
        state = 1; // set the state to 1, cant ro
    }

    // Function to update the vector containing all the sprites
    void updateSprites(vector<Sprite>& sprites)
    {
        sprites = { block1, block2, block3, block4 };
    }

    // Function to determine if a piece is able to rotate
    bool canRotate(float x, float y) {
        return !((x + 35 < 225) || (x - 35 < 225) || (x + 35 > 540) || (x - 35 > 540));
    }

    // Function to correct the state of rotation
    void correctState(const string& type) {
        // Square pieces don't rotate, so no state correction needed
    }


    int toGridX(float x)
    {
        return (x - 225) / 35;
    }

    int toGridY(float y)
    {
        return (y - 50) / 35;
    }

public:

    // Constructor that creates a Square piece in the top middle of the screen
    Square(Texture& gameArt)
    {
        scaleFactor = 5;
        state = 1; // Square pieces don't rotate

        // Initialize block textures and positions
        block1.setTexture(gameArt);
        block2.setTexture(gameArt);
        block3.setTexture(gameArt);
        block4.setTexture(gameArt);

        block1.setTextureRect(IntRect(240, 688, 7, 7));
        block2.setTextureRect(IntRect(240, 688, 7, 7));
        block3.setTextureRect(IntRect(240, 688, 7, 7));
        block4.setTextureRect(IntRect(240, 688, 7, 7));

        block1.setPosition(330, 50);
        block2.setPosition(365, 50);
        block3.setPosition(330, 85);
        block4.setPosition(365, 85);

        block1.setScale(scaleFactor, scaleFactor);
        block2.setScale(scaleFactor, scaleFactor);
        block3.setScale(scaleFactor, scaleFactor);
        block4.setScale(scaleFactor, scaleFactor);

        sprites = { block1, block2, block3, block4 };
    }

    // Getter method that returns the vector containing the four block sprites
    vector<Sprite> getSprites()
    {
        return sprites;
    }

    // Function to rotate the piece (no rotation for square pieces)
    void rotate(string type, vector<vector<int>>& grid)
    {
        // Square pieces cannot rotate
    }

    // Function to move the piece downward
    void fall() {
        for (auto& block : { &block1, &block2, &block3, &block4 }) {
            block->setPosition(block->getPosition().x, block->getPosition().y + 35);
        }
        updateSprites(sprites);
    }

    // Function to move the piece left or right 
    void move(string type) {
        const int xOffset = (type == "left") ? -35 : 35;
        for (auto& block : { &block1, &block2, &block3, &block4 }) {
            block->setPosition(block->getPosition().x + xOffset, block->getPosition().y);
        }
        updateSprites(sprites);
    }

    // Function to determine if the piece can move left at the given moment
    bool canMoveLeft(vector<vector<int>>& grid)
    {
        Time time = clock.getElapsedTime();

        if (time.asMilliseconds() >= 1000 / 2)
        {
            clock.restart();
        }

        for (int i = 0; i < sprites.size(); i++)
        {
            if (sprites[i].getPosition().x == 225)
            {
                return false;
            }

            float x = sprites[i].getPosition().x;
            float y = sprites[i].getPosition().y;

            if (grid[(y - 50) / 35][((x - 225) / 35) - 1] != 0)
            {
                return false;
            }
        }

        if (!canMoveDown(grid) && time.asMilliseconds() >= 500 / 2)
        {
            return false;
        }

        return true;
    }

    // Function to determine if the piece can move right at the given moment
    bool canMoveRight(vector<vector<int>>& grid)
    {
        Time time = clock.getElapsedTime();

        if (time.asMilliseconds() >= 1000 / 2)
        {
            clock.restart();
        }
        for (int i = 0; i < sprites.size(); i++)
        {
            if (sprites[i].getPosition().x == 540)
            {
                return false;
            }

            float x = sprites[i].getPosition().x;
            float y = sprites[i].getPosition().y;

            if (grid[(y - 50) / 35][((x - 225) / 35) + 1] != 0)
            {
                return false;
            }

        }

        if (!canMoveDown(grid) && time.asMilliseconds() >= 500 / 2)
        {
            return false;
        }

        return true;
    }

    // Function to determine if a piece can move down
    bool canMoveDown(vector<vector<int>>& grid)
    {
        for (int i = 0; i < sprites.size(); i++)
        {
            if (sprites[i].getPosition().y == 715)
            {
                return false;
            }

            float x = sprites[i].getPosition().x;
            float y = sprites[i].getPosition().y;

            if (grid[((y - 50) / 35) + 1][(x - 225) / 35] != 0)
            {
                return false;
            }
        }
        return true;
    }

    // Function to update the grid with the correct values
    void updateGrid(vector<vector<int>>& grid)
    {
        for (int i = 0; i < sprites.size(); i++)
        {
            float x = sprites[i].getPosition().x;
            float y = sprites[i].getPosition().y;

            grid[(y - 50) / 35][(x - 225) / 35] = 4; // Square pieces marked as 4 in the grid
        }
    }
};
