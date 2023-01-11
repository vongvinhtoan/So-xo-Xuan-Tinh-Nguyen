#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <set>
#include <fstream>
#include "random.h"

int lock = 0;
std::string state, statelock;
std::set<int> appeared;

std::string num2string(int a)
{
    std::string s;
    for(int i=0; i<3; i++, a/=10) s += a%10 + '0';
    std::reverse(s.begin(), s.end());
    return s;
}

int string2num(std::string s)
{
    int a = 0;
    for(char c: s) a = a*10 + (c - '0');
    return a;
}

void gen(Random &rng)
{
    for(int i=0; i<3; i++) 
    {
        if(lock>>i&1)
            state[i] = statelock[i];
        else 
        {
            state[i] = rng.rand(0, 9) + '0';
        }
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
    Random rng(22112004);

    window->setFramerateLimit(30);

    while(window->isOpen())
    {
        gen(rng);

        for(int i=0; i<3; i++) digits[i].setString(std::string(1, state[i]));

        window->draw(sBackground);
        for(int i=0; i<3; i++) window->draw(digits[i]);

        window->display();
    }
}

void loadData()
{
    std::ifstream fi;
    fi.open("winner.txt");
    int x;
    while(fi>>x)
        appeared.insert(x);
    fi.close();
}

int main()
{
    HWND hWnd = GetForegroundWindow();
    ShowWindow( hWnd, SW_HIDE );

    loadData();

    sf::RenderWindow window(sf::VideoMode(1453, 756), "Lucky Ticket");

    window.setActive(false);

    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    Random rng(22112004);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::ofstream fo;
                fo.open("winner.txt");
                for(int a: appeared) fo<<a<<"\n";
                fo.close();
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int id = 0;
                if(lock == 1) id = 1;
                else if(lock == 3) id = 2;

                if(!lock) statelock = num2string(rng.rand(1, 640));

                lock = lock << 1 | 1;
                lock &= (1<<3) - 1;
            }

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                lock = 0;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S) && lock == ((1<<3)-1))
            {
                appeared.insert(string2num(statelock));
            }
        }
    }

    return 0;
}