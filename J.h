
#include "Piece.h"

class JPiece : public Piece {

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
        if (type == "left")
        {
            state = (state % 4) + 1; 
        }
        else if (type == "right")
        {
            state = (state + 2) % 4 + 1; 
        }
    }

    // Function to update the vector containing all the sprites
    void updateSprites(vector<Sprite>& sprites)
    {
        sprites = { block1, block2, block3, block4 };
    }

    // Function to determine if a piece is able to rotate
    bool canRotate(float x, float y) {
        return (x - 35 >= 225 && x + 35 <= 540);
    }

    // Function to correct the state of rotation
    void correctState(string type) {
        string temp = (type.compare("left") == 0) ? "right" : "left";
        updateState(state, temp);
    }

    // Function to convert x-coordinate to grid position
    int toGridX(float x)
    {
        return (x - 225) / 35;
    }

    // Function to convert y-coordinate to grid position
    int toGridY(float y)
    {
        return (y - 50) / 35;
    }

public:

    // Constructor that creates a JPiece in the top middle of the screen
    JPiece(Texture& gameArt)
    {
        scaleFactor = 5;
        state = 1;

        // Initialize block textures and positions
        block1.setTexture(gameArt);
        block2.setTexture(gameArt);
        block3.setTexture(gameArt);
        block4.setTexture(gameArt);

        block1.setTextureRect(IntRect(240, 728, 7, 7));
        block2.setTextureRect(IntRect(240, 728, 7, 7));
        block3.setTextureRect(IntRect(240, 728, 7, 7));
        block4.setTextureRect(IntRect(240, 728, 7, 7));

        block1.setPosition(330, 50);
        block2.setPosition(365, 50);
        block3.setPosition(400, 50);
        block4.setPosition(400, 85);

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

    // Function to rotate the piece clockwise or counterclockwise
    void rotate(string type, vector<vector<int>>& grid)
    {
        float pivot_x = block2.getPosition().x;
        float pivot_y = block2.getPosition().y;
        int grid_x = toGridX(pivot_x);
        int grid_y = toGridY(pivot_y);

        // Check if rotation is possible
        if (!canRotate(pivot_x, pivot_y)) {
            return;
        }

        // Update rotation state
        updateState(state, type);

        try {
            switch (state) {
            case 1:
                if (grid.at(grid_y).at(grid_x - 1) == 0 &&
                    grid.at(grid_y).at(grid_x + 1) == 0 &&
                    grid.at(grid_y + 1).at(grid_x + 1) == 0) {
                    block1.setPosition(pivot_x - 35, pivot_y);
                    block3.setPosition(pivot_x + 35, pivot_y);
                    block4.setPosition(pivot_x + 35, pivot_y + 35);
                }
                else {
                    correctState(type);
                }
                break;

            case 2:
                if (grid.at(grid_y + 1).at(grid_x) == 0 &&
                    grid.at(grid_y - 1).at(grid_x) == 0 &&
                    grid.at(grid_y - 1).at(grid_x - 1) == 0) {
                    block1.setPosition(pivot_x, pivot_y + 35);
                    block3.setPosition(pivot_x, pivot_y - 35);
                    block4.setPosition(pivot_x - 35, pivot_y + 35);
                }
                else {
                    correctState(type);
                }
                break;

            case 3:
                if (grid.at(grid_y - 1).at(grid_x - 1) == 0 &&
                    grid.at(grid_y).at(grid_x + 1) == 0 &&
                    grid.at(grid_y).at(grid_x - 1) == 0) {
                    block1.setPosition(pivot_x - 35, pivot_y - 35);
                    block3.setPosition(pivot_x + 35, pivot_y);
                    block4.setPosition(pivot_x - 35, pivot_y);
                }
                else {
                    correctState(type);
                }
                break;

            case 4:
                if (grid.at(grid_y - 1).at(grid_x + 1) == 0 &&
                    grid.at(grid_y - 1).at(grid_x) == 0 &&
                    grid.at(grid_y + 1).at(grid_x) == 0) {
                    block1.setPosition(pivot_x + 35, pivot_y - 35);
                    block3.setPosition(pivot_x, pivot_y - 35);
                    block4.setPosition(pivot_x, pivot_y + 35);
                }
                else {
                    correctState(type);
                }
                break;

            default:
                break;
            }
        }
        catch (const out_of_range& e) {
            correctState(type);
        }

        updateSprites(sprites);
    }

    // Function to move the piece downward
    void fall()
    {
        for (auto& block : { &block1, &block2, &block3, &block4 }) {
            block->setPosition(block->getPosition().x, block->getPosition().y + 35);
        }

        updateSprites(sprites);
    }

    // Function to move the piece left or right 
    void move(string type)
    {
        int moveOffset = (type == "left") ? -35 : 35;

        block1.move(moveOffset, 0);
        block2.move(moveOffset, 0);
        block3.move(moveOffset, 0);
        block4.move(moveOffset, 0);

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
        for (const auto& sprite : sprites)
        {
            if (sprite.getPosition().y == 715 || grid[(sprite.getPosition().y - 50) / 35 + 1][(sprite.getPosition().x - 225) / 35] != 0)
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

            grid[(y - 50) / 35][(x - 225) / 35] = 2;
        }
    }
};
