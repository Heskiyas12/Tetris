#include "Menu.h"
#include "Tetris.h" 

Menu::Menu(sf::RenderWindow& window) {
    this->window = &window;
}

void Menu::getHighScores(std::vector<int>& scores, std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        scores.push_back(std::stoi(line));
    }
}

std::string Menu::numAsString(int score, int length) {
    std::string returnString = std::to_string(score);
    int numLeadingZeroes = returnString.size();

    for (int i = 0; i < length - numLeadingZeroes; i++) {
        returnString.insert(returnString.begin(), '0');
    }

    return returnString;
}

//Code that displays the menu screen.
void Menu::displayMenu()
{
    
    Texture blockArt;
    blockArt.loadFromFile("Assets/millgxtg (2).png");

    //Loads up the high scores file and then places them into a vector 
    vector<int> topScores;
    ifstream file("high_scores.txt");
    getHighScores(topScores, file);
    file.close();

    //Sets the portion of the background image that is displayed
    Sprite background;
    background.setTexture(blockArt);
    background.setTextureRect(IntRect(645, 1035 - 750, 800, 750));

    //Displays the black background for the level select portion
    float bx = 125;
    float by = 200;
    RectangleShape levelSelectBG;
    levelSelectBG.setSize(Vector2f(550, 350));
    levelSelectBG.setFillColor(Color::Black);
    levelSelectBG.setPosition(bx, by);

    //Vector to keep all the text for all 10 of the levels displayed
    vector<Text> levelText;
    Font font;
    font.loadFromFile("Assets/text_font.ttf");

   
    float rowShift = 0;
    float x = 0;
    for (int i = 0; i <= 10; i++)
    {
        if (i != 0 && i % 5 == 0)
        {
            rowShift += 100;
            x = 0;
        }
        //This is the text for each level number and the position and number is modified upon each iteration
        Text temp;
        temp.setFont(font);
        temp.setString(to_string(i));
        temp.setPosition((20 + bx) + 120 * x, by + rowShift);
        temp.setFillColor(Color(255, 255, 255));
        temp.setScale(1.3, 1.3);
        x++;

        levelText.push_back(temp);
    }
    //This line makes level 0 red and the one currently selected when the program boots up
    levelText[0].setFillColor(Color(255, 0, 0));

    //booleans to keep track of keyboard input
    bool leftPressed = false;
    bool rightPressed = false;
    //The current level selected in the menu
    int level = 0;

    //This uses the above vector we made containing the top 3 scores and displays them as in game text
   std::vector<sf::Text> highScoreText;
    for (int i = 0; i < 3; i++)
    {
        sf::Text temp;
        temp.setFont(font);
        temp.setString(numAsString(topScores[i], 6));
        temp.setPosition(75, 590 + 50 * i);
        temp.setScale(2, 2);

        highScoreText.push_back(temp);
    }

    // score title 
    sf::Text scoreHeader;
    scoreHeader.setFont(font);
    scoreHeader.setString("Top 3 Scores");
    scoreHeader.setPosition(75, 570);
    scoreHeader.setScale(1.1, 1.1);
    highScoreText.push_back(scoreHeader);

    sf::RectangleShape highScoreBG;
    highScoreBG.setSize(sf::Vector2f(240, 200));
    highScoreBG.setFillColor(sf::Color::Black);
    highScoreBG.setPosition(50, 570);

    // select elvel
    sf::Text levelHeader;
    levelHeader.setFont(font);
    levelHeader.setString("Select Level");
    levelHeader.setPosition(260, 140);
    levelHeader.setScale(1.3, 1.3);

    sf::RectangleShape levelHeaderBG;
    levelHeaderBG.setSize(sf::Vector2f(240, 50));
    levelHeaderBG.setFillColor(sf::Color::Black);
    levelHeaderBG.setPosition(260, 140);

    // made by
    sf::Text madeByText;
    madeByText.setFont(font);
    madeByText.setString("Made  By  Kaleb   Melvin  Heskiyas");
    madeByText.setPosition(200, 90);
    madeByText.setScale(1.0, 1.0);

    sf::RectangleShape madeByBG;
    madeByBG.setSize(sf::Vector2f(450, 40));
    madeByBG.setFillColor(sf::Color::Black);
    madeByBG.setPosition(200, 90);

    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Game     Moves! \nLeft   Arrow \nRight   Arrow \nDown   Arrow \nRotate  Z and X");
    instructionText.setPosition(475, 570);
    instructionText.setScale(1.1, 1.1);

    // background for instruction
    sf::RectangleShape instructionBG;
    instructionBG.setSize(sf::Vector2f(240, 300));
    instructionBG.setFillColor(sf::Color::Black);
    instructionBG.setPosition(475, 570);


    // llop to display menu screen
    while (window->isOpen())
    {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();
        }

        //Code for changing the currently selected level
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !leftPressed)
        {
            levelText[level].setFillColor(Color(255, 255, 255));
            if (level != 0)
            {
                level--;
            }

            else
            {
                level = 10;
            }

            levelText[level].setFillColor(Color(255, 0, 0));
            leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        }

        //Same but if they press right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !rightPressed)
        {
            levelText[level].setFillColor(Color(255, 255, 255));
            if (level != 10)
            {
                level++;
            }

            else
            {
                level = 0;
            }

            levelText[level].setFillColor(Color(255, 0, 0));
            rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);
        }

        //enter= game starts 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            Game game(level, *window);
            game.run();
            break;
        }

        leftPressed = Keyboard::isKeyPressed(sf::Keyboard::Left);
        rightPressed = Keyboard::isKeyPressed(sf::Keyboard::Right);

      // for the drawing
        window->clear();
        window->draw(background);
        window->draw(levelSelectBG);
        for (int i = 0; i < levelText.size(); i++)
        {
            window->draw(levelText[i]);
        }
        window->draw(highScoreBG);
        for (int i = 0; i < highScoreText.size(); i++)
        {
            window->draw(highScoreText[i]);
        }
        window->draw(levelHeaderBG);
        window->draw(levelHeader);
        window->draw(madeByBG); 
        window->draw(madeByText); 
        window->draw(instructionBG); 
        window->draw(instructionText); 
        window->display();
    }
}