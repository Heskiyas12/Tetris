#include "Tetris.h"
#include "RNG.h"

//Initializes empty 10x20 grid.
void Game::initializeGrid(vector<vector<int>>& grid)
{
    grid.resize(20, vector<int>(10, 0));
}

//Helper function to help print current grid for debugging purposes.
void Game::displayGrid(vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row) {
            cout << cell;
        }
        cout << endl;
    }
}


//Function that takes in the grid of pieces and draws the squares that are present at each location.
void Game::drawGameGrid(RenderWindow& window, vector<vector<int>>& grid, Texture& gameArt)
{
    unordered_map<int, IntRect> squares = {
        {1, IntRect(240, 688, 7, 7)},
        {2, IntRect(240, 728, 7, 7)},
        {3, IntRect(240, 768, 7, 7)},
        {4, IntRect(240, 688, 7, 7)},
        {5, IntRect(240, 728, 7, 7)},
        {6, IntRect(240, 768, 7, 7)},
        {7, IntRect(240, 688, 7, 7)}
    };

    float scaleFactor = 5;
    Sprite square;
    square.setTexture(gameArt);
    square.setScale(scaleFactor, scaleFactor);

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] != 0)
            {
                square.setTextureRect(squares[grid[i][j]]);
                square.setPosition(j * 7 * scaleFactor + 225, i * 7 * scaleFactor + 50);
                window.draw(square);
            }
        }
    }
}

//Recursive function that shifts all the lines in the grid down after a line has been cleared.
void Game::shiftLinesDown(vector<vector<int>>& grid, int clearedRow)
{
    if (clearedRow >= 0)
    {
        grid[clearedRow + 1] = grid[clearedRow];
        shiftLinesDown(grid, clearedRow - 1);
    }
}

//Function that determines when a line needs to be cleared.
void Game::clearFullLines(vector<vector<int>>& grid, int level, int& score, int& totalLinesCleared) {
    vector<int> emptyRow(10, 0);
    int linesCleared = 0;
    unordered_map<int, int> pointValues = { {0, 0}, {1, 40}, {2, 100}, {3, 300}, {4, 1200} };

    for (int i = 0; i < grid.size(); i++) {
        if (count(grid[i].begin(), grid[i].end(), 0) == 0) {
            grid[i] = emptyRow;
            shiftLinesDown(grid, i - 1);
            linesCleared++;
        }
    }

    score += pointValues[linesCleared] * (level + 1);
    totalLinesCleared += linesCleared;
    score = min(score, 999999);
}


//Helper function that converts a number into a string of a certain length. For example, integerToString(12,6) would return 000012.
string Game::integerToString(int score, int length)
{
    string returnString = to_string(score);
    returnString = string(max(0, length - static_cast<int>(returnString.size())), '0') + returnString;
    return returnString;
}

//Checks if the game is over by seeing if there are pieces in the top row of the 2D vector.
bool Game::checkGameOver(vector<vector<int>>& grid)
{
    for (int value : grid[0])
    {
        if (value != 0)
        {
            return true;
        }
    }
    return false;
}

//Reads the high_scores.txt file and places the top 3 highest scores into a vector for easy access.
void Game::readTopScores(ifstream& file, vector<int>& scores)
{
    string line;
    while (getline(file, line))
    {
        try
        {
            scores.push_back(stoi(line));
        }
        catch (const invalid_argument& e)
        {
            cerr << "Invalid score format: " << line << endl;
        }
        catch (const out_of_range& e)
        {
            cerr << "Score out of range: " << line << endl;
        }
    }
}

//Updates the high scores after the game is over.
void Game::updateTopScore(vector<int>& scores, int score)
{
    ofstream file("high_scores.txt");
    bool done = false;
    scores.push_back(score);
    std::sort(scores.begin(), scores.end());
    scores.erase(scores.begin());
    std::sort(scores.begin(), scores.end(), greater<>());

    for (int i = 0; i < scores.size(); i++)
    {
        file << scores[i] << endl;
    }
    file.close();
}

/* Initializes the game level and passes the game window in by reference and initializes a pointer that points to
the game window in memory.*/
Game::Game(int level, RenderWindow& window)
{
    this->level = level;
    this->window = &window;
}


void Game::run()
{
    RNG rng;
    vector<vector<int>> grid;
    initializeGrid(grid);

    int score = 0;
    int totalLinesCleared = 0;

    vector<int> highestScores;
    ifstream highScores("high_scores.txt");
    readTopScores(highScores, highestScores);
    highScores.close();

    unordered_map<int, int> statTracker;
    statTracker[1] = 0;
    statTracker[2] = 0;
    statTracker[3] = 0;
    statTracker[4] = 0;
    statTracker[5] = 0;
    statTracker[6] = 0;
    statTracker[7] = 0;

    unordered_map<int, int> levelSpeed;
    int tracker = 1;
    for (int i = 19; i >= 0; i--)
    {
        levelSpeed[i] = 20 * tracker;
        tracker++;
    }

    Texture gameArt;
    gameArt.loadFromFile("Assets/sprites.png");
    Texture background;
    background.loadFromFile("Assets/newpic.jpg");

    bool xPressed = false;
    bool zPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    int keyDelayCounter = 0;
    int keyDelay = 70;

    RectangleShape gameBackground;
    gameBackground.setSize(Vector2f(350, 700));
    gameBackground.setFillColor(Color::Black);
    gameBackground.setPosition(225, 50);

    Sprite* backgroundImage = new Sprite;
    backgroundImage->setTexture(background);
    backgroundImage->setTextureRect(IntRect(0, 0, 800, 750));

    unordered_map<int, IntRect> previewPieces;
    previewPieces[1] = IntRect(240, 688, 23, 15);
    previewPieces[2] = IntRect(240, 728, 23, 15);
    previewPieces[3] = IntRect(240, 768, 23, 15);
    previewPieces[4] = IntRect(244, 808, 15, 15);
    previewPieces[5] = IntRect(240, 848, 23, 15);
    previewPieces[6] = IntRect(240, 888, 23, 15);
    previewPieces[7] = IntRect(276, 932, 31, 7);

    RectangleShape previewSquare;
    previewSquare.setSize(Vector2f(180, 180));
    previewSquare.setFillColor(Color::Black);
    previewSquare.setPosition(600, 300);

    Piece* activePiece = nullptr;

    int firstPiece = rng.randomInt(1, 8);
    int nextPiece = rng.randomInt(1, 8);

    if (firstPiece == 1)
    {
        TPiece* temp = new TPiece(gameArt);
        activePiece = temp;
        statTracker[1]++;
    }

    else if (firstPiece == 2)
    {
        JPiece* temp = new JPiece(gameArt);
        activePiece = temp;
        statTracker[2]++;
    }

    else if (firstPiece == 3)
    {
        ZPiece* temp = new ZPiece(gameArt);
        activePiece = temp;
        statTracker[3]++;
    }

    else if (firstPiece == 4)
    {
        Square* temp = new Square(gameArt);
        activePiece = temp;
        statTracker[4]++;
    }

    else if (firstPiece == 5)
    {
        SPiece* temp = new SPiece(gameArt);
        activePiece = temp;
        statTracker[5]++;
    }

    else if (firstPiece == 6)
    {
        LPiece* temp = new LPiece(gameArt);
        activePiece = temp;
        statTracker[6]++;
    }

    else if (firstPiece == 7)
    {
        Line* temp = new Line(gameArt);
        activePiece = temp;
        statTracker[7]++;
    }

    if (activePiece == nullptr)
    {
        TPiece temp(gameArt);
        activePiece = &temp;
    }

    queue<int> pieceLine;
    pieceLine.push(firstPiece);
    pieceLine.push(nextPiece);

    Sprite previewArt;
    previewArt.setTexture(gameArt);
    previewArt.setTextureRect(previewPieces[nextPiece]);
    previewArt.setPosition(642.5 - 17.5, 385 - 17.5);
    previewArt.setScale(5, 5);

    Clock clock;
    Clock inputTracker;
    int movementFactor = 300;

    Font textFont;
    textFont.loadFromFile("Assets/text_font.ttf");

    RectangleShape scoreBox;
    scoreBox.setSize(Vector2f(180, 180));
    scoreBox.setFillColor(Color::Black);
    scoreBox.setPosition(600, 30);

    Text scoreDisplay;
    scoreDisplay.setFont(textFont);
    scoreDisplay.setString(integerToString(score, 6));
    scoreDisplay.setCharacterSize(50);
    scoreDisplay.setPosition(642.5 - 35, 150);

    Text scoreText;
    scoreText.setFont(textFont);
    scoreText.setString("SCORE");
    scoreText.setCharacterSize(50);
    scoreText.setPosition(642.5 - 22.75, 110);

    Text topScoreDisplay;
    topScoreDisplay.setFont(textFont);
    topScoreDisplay.setString(integerToString(highestScores[0], 6));
    topScoreDisplay.setCharacterSize(50);
    topScoreDisplay.setPosition(642.5 - 35, 70);

    Text topScoreText;
    topScoreText.setFont(textFont);
    topScoreText.setString("TOP");
    topScoreText.setCharacterSize(50);
    topScoreText.setPosition(642.5, 30);

    Text lineText;
    lineText.setFont(textFont);
    lineText.setString("LINES " + integerToString(totalLinesCleared, 3));
    lineText.setCharacterSize(50);
    lineText.setPosition(300, -10);

    vector<Sprite> statPieces;
    int difference = 30;
    for (int i = 1; i <= 7; i++)
    {
        Sprite temp;
        temp.setTexture(gameArt);
        temp.setTextureRect(previewPieces[i]);

        if (i == 7)
        {
            temp.setPosition(60 - difference, 100 + i * 75);
        }

        else
        {
            temp.setPosition(80 - difference, 100 + i * 75);
        }

        temp.setScale(3, 3);
        statPieces.push_back(temp);
    }

    RectangleShape statBox;
    statBox.setSize(Vector2f(190, 525));
    statBox.setFillColor(Color::Black);
    statBox.setPosition(50 - difference, 150);

    vector<Text> statDisplay;

    for (int i = 0; i < 7; i++)
    {
        Text temp;
        temp.setFont(textFont);
        temp.setString(integerToString(0, 3));
        temp.setCharacterSize(50);
        temp.setPosition(642.5, 30);
        temp.setPosition(170 - difference, 160 + i * 75);
        temp.setScale(0.75, 0.75);
        temp.setFillColor(Color(255, 0, 0));

        statDisplay.push_back(temp);
    }

    while (window->isOpen())
    {

        for (int i = 0; i < statDisplay.size(); i++)
        {
            statDisplay[i].setString(integerToString(statTracker[i + 1], 3));
        }

        Time time1 = clock.getElapsedTime();
        Time time2 = clock.getElapsedTime();

        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();
        }

        if (time1.asMilliseconds() >= levelSpeed[level] && time1.asMilliseconds() < 1000 && activePiece->canMoveDown(grid))
        {
            activePiece->fall();
            clock.restart();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !xPressed)
        {

            activePiece->rotate("right", grid);
            xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !zPressed)
        {

            activePiece->rotate("left", grid);
            zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && activePiece->canMoveLeft(grid))
        {
            keyDelayCounter++;

            if (keyDelayCounter == keyDelay)
            {
                activePiece->move("left");
                leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
                keyDelayCounter = 0;
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && activePiece->canMoveRight(grid))
        {
            keyDelayCounter++;

            if (keyDelayCounter == keyDelay)
            {
                activePiece->move("right");
                rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
                keyDelayCounter = 0;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && activePiece->canMoveDown(grid))
        {
            keyDelayCounter++;

            if (keyDelayCounter == keyDelay)
            {
                activePiece->fall();
                leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
                keyDelayCounter = 0;
            }

        }

        xPressed = Keyboard::isKeyPressed(sf::Keyboard::X);
        zPressed = Keyboard::isKeyPressed(sf::Keyboard::Z);
        leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

        if (!activePiece->canMoveDown(grid) && !checkGameOver(grid) && !activePiece->canMoveLeft(grid) && !activePiece->canMoveRight(grid))
        {
            pieceLine.pop();
            int previewPiece = rng.randomInt(1, 8);
            pieceLine.push(previewPiece);
            int generatedPiece = pieceLine.front();
            previewArt.setTextureRect(previewPieces[previewPiece]);

            activePiece->updateGrid(grid);
            clearFullLines(grid, level, score, totalLinesCleared);
            scoreDisplay.setString(integerToString(score, 6));
            lineText.setString("LINES " + integerToString(totalLinesCleared, 3));

            delete activePiece;

            if (generatedPiece == 1)
            {
                TPiece* temp2 = new TPiece(gameArt);
                activePiece = temp2;
                statTracker[1]++;
            }

            else if (generatedPiece == 2)
            {
                JPiece* temp2 = new JPiece(gameArt);
                activePiece = temp2;
                statTracker[2]++;
            }

            else if (generatedPiece == 3)
            {
                ZPiece* temp2 = new ZPiece(gameArt);
                activePiece = temp2;
                statTracker[3]++;
            }

            else if (generatedPiece == 4)
            {
                Square* temp2 = new Square(gameArt);
                activePiece = temp2;
                statTracker[4]++;
            }

            else if (generatedPiece == 5)
            {
                SPiece* temp2 = new SPiece(gameArt);
                activePiece = temp2;
                statTracker[5]++;
            }

            else if (generatedPiece == 6)
            {
                LPiece* temp2 = new LPiece(gameArt);
                activePiece = temp2;
                statTracker[6]++;
            }

            else if (generatedPiece == 7)
            {
                Line* temp2 = new Line(gameArt);
                activePiece = temp2;
                statTracker[7]++;
            }

        }

        else if (checkGameOver(grid))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                ofstream writeHighScore("MISC/high_scores.txt");
                updateTopScore(highestScores, score);

                for (int i = 0; i < grid.size(); i++)
                {
                    for (int j = 0; j < grid[i].size(); j++)
                    {
                        grid[i][j] = 0;
                    }
                }
                score = 0;
                totalLinesCleared = 0;
                scoreDisplay.setString(integerToString(0, 6));
                lineText.setString("LINES " + integerToString(totalLinesCleared, 3));
                statTracker[1] = 0;
                statTracker[2] = 0;
                statTracker[3] = 0;
                statTracker[4] = 0;
                statTracker[5] = 0;
                statTracker[6] = 0;
                statTracker[7] = 0;

                for (int i = 0; i < statDisplay.size(); i++)
                {
                    statDisplay[i].setString(integerToString(0, 3));
                }
                clock.restart();

                Menu menu(*window);
                menu.displayMenu();
                window->close();
                break;
            }

        }

        window->clear(Color(0, 0, 255));

        window->draw(*backgroundImage);
        window->draw(gameBackground);
        drawGameGrid(*window, grid, gameArt);
        vector<Sprite> v = activePiece->getSprites();
        for (int i = 0; i < v.size(); i++)
        {
            window->draw(v[i]);
        }
        window->draw(previewSquare);
        window->draw(previewArt);
        window->draw(scoreBox);
        window->draw(scoreDisplay);
        window->draw(scoreText);
        window->draw(topScoreDisplay);
        window->draw(topScoreText);
        window->draw(lineText);

        window->draw(statBox);
        for (int i = 0; i < statPieces.size(); i++)
        {
            window->draw(statPieces[i]);
        }

        for (int i = 0; i < statDisplay.size(); i++)
        {
            window->draw(statDisplay[i]);
        }

        window->display();

    }

    delete backgroundImage;

}