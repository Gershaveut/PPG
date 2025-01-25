#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

int main() {
  unsigned int width = 400;
  unsigned int height = 200;

  float playerSpeed = 1.50;

  auto window = sf::RenderWindow(sf::VideoMode({width, height}), "PPG");
  window.setFramerateLimit(120);

  sf::Vector2f ballDirection;
  ballDirection = {1, -1};

  sf::CircleShape ball(25.f);

  float ballScale = ball.getScale().x;

  ball.setOrigin({ballScale / 2, ballScale / 2});
  ball.setPosition({width / 2.f, width / 2.f});

  sf::Vector2f playerShape({15, 50});

  sf::RectangleShape player(playerShape);
  sf::RectangleShape playerBot(playerShape);
  player.setPosition({25, height / 2.f});
  playerBot.setPosition({width - 25.f, height / 2.f});

  sf::RectangleShape screen({width - ballScale, height - ballScale});

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      player.move({0, 1 * playerSpeed});
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      player.move({0, -1 * playerSpeed});
    }

    if (playerBot.getPosition().y < ball.getPosition().y) {
      playerBot.move({0, 1 * playerSpeed});
    } else {
      playerBot.move({0, -1 * playerSpeed});
    }

    auto ballBounds = ball.getGlobalBounds();

    if (ballBounds.findIntersection(player.getGlobalBounds()) ||
        ballBounds.findIntersection(playerBot.getGlobalBounds())) {
      ballDirection.x *= -1;
    }

    if (!ballBounds.findIntersection(screen.getGlobalBounds())) {
      if (ball.getPosition().x > screen.getSize().x) {
        ballDirection.x = -1;
      } else if (ball.getPosition().x < 0) {
        ballDirection.x = 1;
      } else if (ball.getPosition().y > screen.getSize().y) {
        ballDirection.y = -1;
      } else {
        ballDirection.y = 1;
      }
    }

    ball.move(ballDirection);

    window.clear();
    window.draw(ball);
    window.draw(player);
    window.draw(playerBot);
    window.display();
  }
}
