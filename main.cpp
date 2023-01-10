#include <SFML/Graphics.hpp>

sf::CircleShape shape(100, 10000);

void renderingThread(sf::RenderWindow* window)
{
    shape.setFillColor(sf::Color::Green);
    
    window->setActive(true);

    while (window->isOpen())
    {
        window->draw(shape);

        window->display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Haha");

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