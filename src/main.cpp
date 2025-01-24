#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

int main()
{
    unsigned int width = 1920;
    unsigned int height = 1080;
    
    auto window = sf::RenderWindow(sf::VideoMode({width, height}), "PPG");
    window.setFramerateLimit(144);
    
    sf::Vector2f ballDirection;
    ballDirection = {1, -1};
    
    sf::CircleShape ball(25.f);

    float ballScale = ball.getScale().x;
    
    ball.setOrigin({ballScale / 2, ballScale / 2});
    ball.setPosition({50, 50});
        
    sf::RectangleShape screen({width - ballScale, height - ballScale});
        
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        auto ballBounds = ball.getGlobalBounds();

        if (!ballBounds.findIntersection(screen.getGlobalBounds()))
        {
            if (ball.getPosition().x > screen.getSize().x)
            {
                ballDirection.x = -1;
            }
            else if (ball.getPosition().x < 0)
            {
                ballDirection.x = 1;
            }
            else
            {
                ballDirection.y *= -1;
            }
        }
        
        ball.move(ballDirection);

        window.clear();
        window.draw(ball);
        window.display();
    }
}
