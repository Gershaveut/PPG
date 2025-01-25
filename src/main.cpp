#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>
#include <random>
#include <string>

unsigned int updateSpeed = 500; // 120

unsigned int width = 1920;
unsigned int height = 1080;

auto window = sf::RenderWindow(sf::VideoMode({width, height}), "PPG");

sf::Vector2f center({width / 2.f, height / 2.f});

float playerSpeed = 1.50;
float playerBotSlowing = 1.50;
unsigned int playerOffset = 100;

unsigned int playerScore;
unsigned int playerBotScore;

sf::Vector2f playerShape({15, 50});

sf::RectangleShape player(playerShape);
sf::RectangleShape playerBot(playerShape);

sf::CircleShape ball(10.f);

float ballSpeed = 1.25;

sf::Vector2f ballDirection;

sf::Font font("arial.ttf");
sf::Text score(font, "0 : 0");

void setUpdateSpeed(int speed) {
  updateSpeed = speed;
  window.setFramerateLimit(speed);
}

void clearScreen() {
  ball.setPosition({center.x, center.y});

  player.setPosition({(float)playerOffset, center.y});
  playerBot.setPosition({(float)width - playerOffset, center.y});

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> dis(0, 200);

  ballDirection = {(dis(gen) - 50) / 100, (dis(gen) - 100) / 100};
  ballDirection = ballDirection.normalized();
}

void updateScore() {
  clearScreen();

  setUpdateSpeed(updateSpeed + 25);

  score.setString(std::to_string(playerScore) + " : " +
                  std::to_string(playerBotScore));
}

int main() {
  window.setFramerateLimit(updateSpeed);

  clearScreen();

  score.setPosition({center.x, 0});

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    sf::Vector2f ballPosition = ball.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      player.move({0, 1 * playerSpeed});
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      player.move({0, -1 * playerSpeed});
    }

    if (playerBot.getPosition().y < ballPosition.y) {
      playerBot.move({0, 1 * playerSpeed / playerBotSlowing});
    } else {
      playerBot.move({0, -1 * playerSpeed / playerBotSlowing});
    }

    auto ballBounds = ball.getGlobalBounds();

    if (ballBounds.findIntersection(player.getGlobalBounds()) ||
        ballBounds.findIntersection(playerBot.getGlobalBounds())) {
      ballDirection.x *= -1;
    }

    if (ballPosition.x > width) {
      ++playerScore;
      updateScore();
    } else if (ballPosition.x < 0) {
      ++playerBotScore;
      updateScore();
    } else if (ballPosition.y > height) {
      ballDirection.y = -1;
    } else if (ballPosition.y < 0) {
      ballDirection.y = 1;
    }

    ball.move(ballDirection * ballSpeed);

    window.clear();
    window.draw(ball);
    window.draw(player);
    window.draw(playerBot);
    window.draw(score);
    window.display();
  }
}
