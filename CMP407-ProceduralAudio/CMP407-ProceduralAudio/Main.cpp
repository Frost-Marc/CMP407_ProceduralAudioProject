#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "ProceduralAudio");

    sf::CircleShape circle;
    circle.setRadius(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({ 200,200 });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

	return 0;
}