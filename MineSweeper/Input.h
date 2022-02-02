#pragma once

#include <SFML/Graphics.hpp>

// Definition

// Keyboard
#define KEY_SPACE sf::Keyboard::isKeyPressed(sf::Keyboard::Space)

#define KEY_M sf::Keyboard::isKeyPressed(sf::Keyboard::M)
#define KEY_R sf::Keyboard::isKeyPressed(sf::Keyboard::R)
#define KEY_S sf::Keyboard::isKeyPressed(sf::Keyboard::S)

// Mouse
#define BTN_LEFT sf::Mouse::isButtonPressed(sf::Mouse::Left)
#define BTN_RIGHT sf::Mouse::isButtonPressed(sf::Mouse::Right)
#define BTN_MIDDLE sf::Mouse::isButtonPressed(sf::Mouse::Middle)