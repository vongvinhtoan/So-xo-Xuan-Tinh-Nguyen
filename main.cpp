#include <SFML/Graphics.hpp>
#include <assert.h>

void renderingThread(sf::RenderWindow* window)
{
    sf::Texture tBackground;
    tBackground.loadFromFile("sprites/Lucky.png"); //1453 x 756
    tBackground.setSmooth(1);

    sf::Sprite sBackground;
    sBackground.setTexture(tBackground);
    sBackground.scale(sf::Vector2f(.8, .8));

    sf::Font font;
    font.loadFromFile("fonts/DejaVuSerif.ttf");

    window->setActive(true);

    while (window->isOpen())
    {
        window->draw(sBackground);

        window->display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1453*0.8, 756*0.8), "Haha");

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
        }
    }

    return 0;
}