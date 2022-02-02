#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class MineSweeper {

	public:
		MineSweeper(sf::Vector2f Iposition, sf::Vector2f Isize, int IboardSize) {

			gameX = Iposition.x;
			gameY = Iposition.y;
			gameWidth = Isize.x;
			gameHeight = Isize.y;

			boardSize = IboardSize;

		}

		void draw(sf::RenderWindow &window) {

			window.draw(background);

		}

	private:
		sf::RectangleShape background;

		float gameX;
		float gameY;
		float gameWidth;
		float gameHeight;

		std::vector<std::vector<int>> boardPosition;
		int boardSize;
};