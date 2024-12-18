#include "Tetris.h"
#include "Menu.h"  

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 750), "Tetris");

   
    Menu menu(window);

    menu.displayMenu(); 
    return 0;
}