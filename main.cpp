#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include "random.h"

int lock = 0;

void gen(std::string &state, Random &rng)
{
    for(int i=0; i<3; i++) 
    {
        if(lock>>i&1)
            state[i] = '#';
        else 
            state[i] = rng.rand(0, 9) + '0';
    }
}

void renderingThread(sf::RenderWindow* window)
{
    sf::Texture tBackground;
    tBackground.loadFromFile("sprites/Lucky.png"); //1453 x 756
    tBackground.setSmooth(1);

    sf::Sprite sBackground;
    sBackground.setTexture(tBackground);
    // sBackground.scale(sf::Vector2f(.8, .8));

    sf::Font font;
    font.loadFromFile("fonts/DejaVuSerifCondensed-Bold.ttf");

    sf::Text digits[3];

    for(int i=0; i<3; i++)
    {
        sf::Text &text = digits[i];

        text.setFont(font);
        text.setCharacterSize(160);
        text.setFillColor(sf::Color(190, 38, 51));
        text.setPosition(sf::Vector2f(854 + 143 * i, 384));
    }

    std::string state;
    Random rng(11235813);

    window->setFramerateLimit(30);

    while(window->isOpen())
    {
        gen(state, rng);

        for(int i=0; i<3; i++) digits[i].setString(std::string(1, state[i]));

        window->draw(sBackground);
        for(int i=0; i<3; i++) window->draw(digits[i]);

        window->display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1453, 756), "Haha");

    window.setActive(false);

    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                lock = lock << 1 | 1;
                lock &= (1<<3) - 1;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                lock = 0;
            }
        }
    }

    return 0;
}