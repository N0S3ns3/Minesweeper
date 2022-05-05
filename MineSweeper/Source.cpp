#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Input.h"

// Window
bool window_Fullscreen = false;
bool window_Redraw = true;
std::vector<std::vector<int>> window_Ratio = { {16, 9} };
std::string window_Version = "V1.1.0";

int window_Size = 75;
int window_Type = 0;
float window_Height;
float window_Width;
float window_Scale;

// Mouse
float mouse_x = 0;
float mouse_y = 0;

class MineSweeper {

public:

	class infoPanel {

	public:
		infoPanel() {

			T_Background2r1.loadFromFile("Style/Texture/Background/TileBackground[2][1].jpg");
			font.loadFromFile("Style/Font/digital7.ttf");

		}

		// x25 y30 w220 h110
		void resize(float x, float y, float w, float h) {

			panel.setSize(sf::Vector2f(w * window_Scale, h * window_Scale));
			panel.setPosition(sf::Vector2f(x, y));
			panel.setFillColor(sf::Color::Black);

			Background2r1.setTexture(T_Background2r1);
			Background2r1.setScale(sf::Vector2f(0.001 * (panel.getSize().x + 10 * window_Scale), 0.002 * (panel.getSize().y + 10 * window_Scale)));
			Background2r1.setPosition(sf::Vector2f(panel.getPosition().x - 5 * window_Scale, panel.getPosition().y - 5 * window_Scale));

			text.setFont(font);
			text.setString("888");
			text.setCharacterSize(100 * window_Scale);
			text.setPosition(sf::Vector2f((panel.getSize().x / 2 - text.getLocalBounds().width / 2) + panel.getPosition().x - 5 * window_Scale, panel.getPosition().y - 9 * window_Scale));
			text.setFillColor(sf::Color::Color(60, 0, 0));

		}

		void setText(int input) {

			number = input;
			sText = std::to_string(number);

		}

		void draw(sf::RenderWindow& window) {

			window.draw(Background2r1);
			window.draw(panel);

			text.setString("888");
			text.setFillColor(sf::Color::Color(60, 0, 0));
			window.draw(text);

			// You realise there are 2 set string and color. It's because to get cool visual effect that looks like real digital clock
			std::string zeros;
			for (int i = 0; i < 3 - sText.size(); i++) {
				zeros += "0";
			}
			text.setString(zeros + sText);

			text.setFillColor(sf::Color::Color(200, 0, 0));

			window.draw(text);

		}

	private:
		sf::RectangleShape panel;

		sf::Text text;
		sf::Font font;

		sf::Sprite Background2r1;

		sf::Texture T_Background2r1;

		int number = 0;
		std::string sText;

	};

	class buttonPanel {

		public:
			void loadTexture(std::string file) {

				T_Unselected.loadFromFile("Style/Texture/Buttons/Unselected/" + file);
				T_Selected.loadFromFile("Style/Texture/Buttons/Selected/" + file);
				button.setTexture(T_Unselected);

			}

			void resize(float x, float y, float w, float h, bool c) {

				button.setScale(sf::Vector2f(0.001 * w * window_Scale, 0.002 * h * window_Scale));
				button.setPosition(sf::Vector2f(x * window_Scale - (c * w / 2 * window_Scale), y * window_Scale));

			}

			void setSelected() {
				button.setTexture(T_Selected);
			}

			void setUnselected() {
				button.setTexture(T_Unselected);
			}

			bool select() {

				if (button.getPosition().x < mouse_x && button.getPosition().x + button.getScale().x / 0.001 > mouse_x) {
					if (button.getPosition().y < mouse_y && button.getPosition().y + button.getScale().y / 0.002 > mouse_y) {
						return true;
					}
				}
				return false;

			}

			void draw(sf::RenderWindow &window) {

				window.draw(button);

			}

		private:
			sf::Sprite button;
			sf::Texture T_Selected;
			sf::Texture T_Unselected;

			bool selected = false;

	};

	// Basically setup game upon creating it and load textures before using/ destroying them
	MineSweeper(sf::Vector2f Iposition, sf::Vector2f Isize, sf::Vector2f Ioffset) {

		gameX = Iposition.x;
		gameY = Iposition.y;
		gameOffsetX = Ioffset.x;
		gameOffsetY = Ioffset.y;
		gameWidth = Isize.x;
		gameHeight = Isize.y;

		// Load sounds from folder
		S_Uncover.loadFromFile("Sound/Select/Uncover.wav");
		S_Flag.loadFromFile("Sound/HitHurt/Flag.wav");
		S_Load.loadFromFile("Sound/Synth/load.wav");
		S_Win.loadFromFile("Sound/Synth/win.wav");
		S_Explode.loadFromFile("Sound/Explosion/explosion.wav");
		S_Press.loadFromFile("Sound/Select/Press.wav");

		// Load texture from folder
		T_Bomb.loadFromFile("Style/Texture/Tiles/State/TileState_1.jpg");
		T_Cover.loadFromFile("Style/Texture/Tiles/Cover/TileCover_0.jpg");
		T_Uncover.loadFromFile("Style/Texture/Tiles/Cover/TileCover_1.jpg");
		T_Flag.loadFromFile("Style/Texture/Tiles/Cover/TileCover_2.jpg");
		T_Background1r1.loadFromFile("Style/Texture/Background/TileBackground[1][1].jpg");
		
		T_Cover_1.loadFromFile("Style/Texture/Tiles/N/TileCoverN_1.jpg");
		T_Cover_2.loadFromFile("Style/Texture/Tiles/N/TileCoverN_2.jpg");
		T_Cover_3.loadFromFile("Style/Texture/Tiles/N/TileCoverN_3.jpg");
		T_Cover_4.loadFromFile("Style/Texture/Tiles/N/TileCoverN_4.jpg");
		T_Cover_5.loadFromFile("Style/Texture/Tiles/N/TileCoverN_5.jpg");
		T_Cover_6.loadFromFile("Style/Texture/Tiles/N/TileCoverN_6.jpg");
		T_Cover_7.loadFromFile("Style/Texture/Tiles/N/TileCoverN_7.jpg");
		T_Cover_8.loadFromFile("Style/Texture/Tiles/N/TileCoverN_8.jpg");

		setupTile();

		// Panels
		flagPanel.resize(25, 30, 220, 110);
		timePanel.resize(window_Width - 220 - 25, 30, 220, 110);

		B_Easy.loadTexture("Easy.jpg");
		B_Medium.loadTexture("Medium.jpg");
		B_Hard.loadTexture("Hard.jpg");
		B_Brave.loadTexture("Brave.jpg");
		B_IWarnedYou.loadTexture("IWarnedYou.jpg");
		B_GetLost.loadTexture("GetLost.jpg");
		
		B_Easy.resize(25, 180, 220, 110, false);
		B_Medium.resize(25, 320, 220, 110, false);
		B_Hard.resize(25, 460, 220, 110, false);
		B_Brave.resize(window_Width - 220 - 25, 180, 220, 110, false);
		B_IWarnedYou.resize(window_Width - 220 - 25, 320, 220, 110, false);
		B_GetLost.resize(window_Width - 220 - 25, 460, 220, 110, false);

		B_Easy.setSelected();

	}

	// Set gamePosition
	void setPosition(sf::Vector2f Iposition) {

		gameX = Iposition.x / window_Scale;
		gameY = Iposition.y / window_Scale;

	}

	// Set gameSize
	void setSize(sf::Vector2f Isize) {

		game.setSize(Isize);

	}

	// Set linePosition
	void lineSetPosition(sf::Vector2f linePosition1, sf::Vector2f linePosition2) {

		line[0] = linePosition1;
		line[1] = linePosition2;
		lineSetColor(lineC, lineC);

	}

	// Set lineColor
	void lineSetColor(sf::Color lineColor1, sf::Color lineColor2) {

		line[0].color = lineColor1;
		line[1].color = lineColor2;

	}

	// Setup tiles by setting all tiles clear/covered/unchecked and randomize positions for the bomb and set gameState true
	void setupTile() {

		boardSize = LvlSize[Lvl][0];
		boardBomb = LvlSize[Lvl][1];

		game.setPosition(sf::Vector2f((gameX + gameOffsetX) * window_Scale, (gameY + gameOffsetY) * window_Scale));
		game.setSize(sf::Vector2f(gameWidth * window_Scale, gameHeight * window_Scale));

		tile.setScale(sf::Vector2f(game.getSize().x / boardSize / 300, game.getSize().y / boardSize / 300));

		for (int y = 0; y < 200; y++) {

			for (int x = 0; x < 200; x++) {

				tileState[y][x] = CLEAR;
				tileCover[y][x] = COVERED;
				tileCheck[y][x] = UNCHECKED;

			}

		}

		srand((unsigned) time(0));
		int i = 0;
		while (i < boardBomb) {

			for (int y = 0; y < boardSize; y++) {

				for (int x = 0; x < boardSize; x++) {

					if (i >= boardBomb) { break; }

					if (rand() % 10 == 5 && tileState[y][x] != BOMB) {

						i++;
						tileState[y][x] = BOMB;

					}

				}

			}

		}

		gameState = true;
		timer = false;
		gameTime = 0;

		flag = boardBomb;

	}
	
	void restart() {

		if (gameSound) {
			sound.setBuffer(S_Load);
			sound.play();
		}

		setupTile();
		timer = false;

	}

	// Set state of the tileState
	void setTileState(int x, int y, int state) {

		// 0 = Clear
		// 1 = Bomb

		tileState[y][x] = state;

	}

	// Set State of the tileCover
	void setTileCover(int x, int y, int state) {

		// 0 = Covered
		// 1 = Uncovered
		// 2 = Flagged

		tileCover[y][x] = state;

	}

	// Get state of the tileState
	int getTileState(int x, int y) {

		// 0 = Clear
		// 1 = Bomb

		return tileState[y][x];

	}

	// Get state of the tileCover
	int getTileCover(int x, int y) {

		// 0 = Covered
		// 1 = Uncovered
		// 2 = Flagged

		return tileCover[y][x];

	}

	// Count how many bomb specific tile has nearby
	int countBomb(int board[200][200], int x, int y) {

		int count = 0;

		for (int d = 0; d < sizeof(direction) / sizeof(direction[0]); d++) {

			if (board[y + direction[d][0]][x + direction[d][1]] == BOMB) {

				count++;

			}

		}

		return count;

	}

	// Find bomb that has been uncovered, and set gameState false(also lost game)
	void findBomb() {

		if (gameState) {

			for (int y = 0; y < boardSize; y++) {
				for (int x = 0; x < boardSize; x++) {

					if (tileState[y][x] == BOMB && tileCover[y][x] == UNCOVERED) {

						uncoverAllTile();
						gameState = false;
						timer = false;

						if (gameSound) {
							sound.setBuffer(S_Explode);
							sound.play();
						}

					}

				}
			}

		}

	}

	// Check uncovered tile and uncover neighbourhooding tile that doesn't have bomb nearby
	void findUncovered() {

		int count = 0;

		if (gameState) {

			do {

				count = 0;

				for (int y = 0; y < boardSize; y++) {

					for (int x = 0; x < boardSize; x++) {

						if (tileCover[y][x] == UNCOVERED && countBomb(tileState, x, y) <= 0) {

							if (tileCheck[y][x] == UNCHECKED) {

								for (int d = 0; d < sizeof(direction) / sizeof(direction[0]); d++) {

									if (tileCover[y + direction[d][0]][x + direction[d][1]] == COVERED) {

										tileCover[y + direction[d][0]][x + direction[d][1]] = UNCOVERED;
										count++;

									}

								}

								tileCheck[y][x] = CHECKED;

							}

						}

					}

				}

			} while (count > 0);

		}

	}

	// Uncover specific tile using [x y] coordinates
	void uncoverTile(float Ix, float Iy) {

		if (gameState) {

			float xStep = game.getSize().x / boardSize;
			float yStep = game.getSize().y / boardSize;

			for (int y = 0; y < boardSize; y++) {

				for (int x = 0; x < boardSize; x++) {

					if (boxCollisionTrigger(x * xStep + game.getPosition().x, y * yStep + game.getPosition().y, game.getSize().x / boardSize, game.getSize().y / boardSize, Ix, Iy)) {

						if (tileCover[y][x] != FLAGGED && tileCover[y][x] != UNCOVERED) {

							tileCover[y][x] = UNCOVERED;

							findUncovered();

							if (!timer && gameState) {

								timer = true;
								gameTimer.restart();

							}

							if (gameSound) {
								sound.setBuffer(S_Uncover);
								sound.play();
							}

						}

					}

				}

			}

		}

	}

	// Uncover all tile, including bomb and flags
	void uncoverAllTile() {

		for (int y = 0; y < boardSize; y++) {

			for (int x = 0; x < boardSize; x++) {

				if (tileCover[y][x] != FLAGGED) {

					tileCover[y][x] = 1;

				}
				
				if (tileCover[y][x] == FLAGGED && tileState[y][x] != BOMB) {

					tileCover[y][x] = 1;

				}

			}

		}

	}

	// Toggle flag on specific tile while also helping to prevent uncovering flagged tile
	void flagTile(float Ix, float Iy) {

		if (gameState) {

			float xStep = game.getSize().x / boardSize;
			float yStep = game.getSize().y / boardSize;

			for (int y = 0; y < boardSize; y++) {

				for (int x = 0; x < boardSize; x++) {

					if (boxCollisionTrigger(x * xStep + game.getPosition().x, y * yStep + game.getPosition().y, game.getSize().x / boardSize, game.getSize().y / boardSize, Ix, Iy)) {

						if (tileCover[y][x] == COVERED) {

							tileCover[y][x] = FLAGGED;
							flag--;

						}
						else if (tileCover[y][x] == FLAGGED) {

							tileCover[y][x] = COVERED;
							flag++;

						}

						if (!timer && gameState) {

							timer = true;
							gameTimer.restart();

						}

						if (gameSound && tileCover[y][x] == COVERED || tileCover[y][x] == FLAGGED) {
							sound.setBuffer(S_Flag);
							sound.play();
						}

					}

				}

			}

		}

	}

	// For debugging purpose
	void displayTile(int board[200][200]) {

		for (int y = 0; y < boardSize; y++) {
			for (int x = 0; x < boardSize; x++) {

				std::cout << board[y][x];

			}

			std::cout << std::endl;
		}

		std::cout << std::endl;

	}

	// Check collision trigger using [x y w h] and [x2 y2]
	bool boxCollisionTrigger(float x, float y, float w, float h, float x2, float y2) {

		return x < x2 && x + w > x2 && y < y2 && y + h > y2;

	}

	// Check if player has erased all tile excluding bomb
	void checkWin() {

		if (gameState) {

			int count = 0;

			for (int y = 0; y < boardSize; y++) {
				for (int x = 0; x < boardSize; x++) {

					if (tileCover[y][x] == COVERED || tileCover[y][x] == FLAGGED) {

						count++;

					}

				}
			}

			if (count == boardBomb) {

				gameState = false;
				timer = false;

				sound.setBuffer(S_Win);
				sound.play();

			}

		}

	}

	// Draw tiles before grid with lines
	void drawGrid(sf::RenderWindow &window) {

		float xStep = game.getSize().x / boardSize;
		float yStep = game.getSize().y / boardSize;

		drawTile(window);

		for (int y = 0; y < boardSize+1; y++) {

			// Horizontal
			lineSetPosition(sf::Vector2f(game.getPosition().x, y * yStep + game.getPosition().y), sf::Vector2f(game.getSize().x + game.getPosition().x, y * yStep + game.getPosition().y));
			window.draw(line, 2, sf::Lines);

			for (int x = 0; x < boardSize+1; x++) {

				// Vertical
				lineSetPosition(sf::Vector2f(x * xStep + game.getPosition().x, game.getPosition().y), sf::Vector2f(x * xStep + game.getPosition().x, game.getSize().y + game.getPosition().y));
				window.draw(line, 2, sf::Lines);

			}

		}

	}

	// Draw background for the main game
	void drawBackground(sf::RenderWindow &window) {

		Background1r1.setTexture(T_Background1r1);
		Background1r1.setScale(sf::Vector2f(game.getSize().x / 950, game.getSize().y / 950));
		Background1r1.setPosition(sf::Vector2f(game.getPosition().x - 25 * Background1r1.getScale().x, game.getPosition().y - 25 * Background1r1.getScale().y));

		window.draw(Background1r1);

	}

	// Get gameState
	bool getGameState() {

		return gameState;

	}

	// Toggle sound (on/off)
	void toggleSound() {

		gameSound = !gameSound;

	}

	void setLevel() {

		if (B_Easy.select() || B_Medium.select() || B_Hard.select() || B_Brave.select() || B_IWarnedYou.select() || B_GetLost.select()) {
			B_Easy.setUnselected();
			B_Medium.setUnselected();
			B_Hard.setUnselected();
			B_Brave.setUnselected();
			B_IWarnedYou.setUnselected();
			B_GetLost.setUnselected();

			sound.setBuffer(S_Press);
			sound.play();

		}

		int tmpLvl = Lvl;
		int tmpTime = gameTime;

		if (B_Easy.select()) {
			B_Easy.setSelected();
			Lvl = 0;
			setupTile();
		}
		if (B_Medium.select()) {
			B_Medium.setSelected();
			Lvl = 1;
			setupTile();
		}
		if (B_Hard.select()) {
			B_Hard.setSelected();
			Lvl = 2;
			setupTile();
		}
		if (B_Brave.select()) {
			B_Brave.setSelected();
			Lvl = 3;
			setupTile();
		}
		if (B_IWarnedYou.select()) {
			B_IWarnedYou.setSelected();
			Lvl = 4;
			setupTile();
		}
		if (B_GetLost.select()) {
			B_GetLost.setSelected();
			Lvl = 5;
			setupTile();
		}

		if (Lvl == tmpLvl) {

			gameTime = tmpTime;

		}

	}

	// Resize so the object are fixed size with any screen size with ratio 16:9
	void resize() {

		game.setPosition(sf::Vector2f((gameX + gameOffsetX) * window_Scale, (gameY + gameOffsetY) * window_Scale));
		game.setSize(sf::Vector2f(gameWidth * window_Scale, gameHeight * window_Scale));
		tile.setScale(sf::Vector2f(game.getSize().x / boardSize / 300, game.getSize().y / boardSize / 300));

		flagPanel.resize(25 * window_Scale, 30 * window_Scale, 220, 110);
		timePanel.resize(window_Width - 220 * window_Scale - 25 * window_Scale, 30 * window_Scale, 220, 110);

		B_Easy.resize(25, 180, 220, 110, false);
		B_Medium.resize(25, 320, 220, 110, false);
		B_Hard.resize(25, 460, 220, 110, false);
		B_Brave.resize(window_Width / window_Scale - 220 - 25, 180, 220, 110, false);
		B_IWarnedYou.resize(window_Width / window_Scale - 220 - 25, 320, 220, 110, false);
		B_GetLost.resize(window_Width / window_Scale - 220 - 25, 460, 220, 110, false);

	}

	// Draw tiles with different textures while checking bomb and win.
	void drawTile(sf::RenderWindow &window) {

		float xStep = game.getSize().x / boardSize;
		float yStep = game.getSize().y / boardSize;

		findBomb();
		checkWin();

		for (int y = 0; y < boardSize; y++) {
			for (int x = 0; x < boardSize; x++) {

				tile.setPosition(sf::Vector2f(x * xStep + game.getPosition().x, y * yStep + game.getPosition().y));

				if (tileCover[y][x] != UNCOVERED) {

					if (tileCover[y][x] == COVERED) {

						tile.setTexture(T_Cover);

					}

					if (tileCover[y][x] == FLAGGED) {

						tile.setTexture(T_Flag);

					}

					window.draw(tile);

				}
				else if (tileState[y][x] == BOMB) {

					tile.setTexture(T_Bomb);
					window.draw(tile);

				}
				else {

					if (countBomb(tileState, x, y) > 0) {

						int count = countBomb(tileState, x, y);

						switch (count) {

						case 1:
							tile.setTexture(T_Cover_1);
							break;

						case 2:
							tile.setTexture(T_Cover_2);
							break;

						case 3:
							tile.setTexture(T_Cover_3);
							break;

						case 4:
							tile.setTexture(T_Cover_4);
							break;

						case 5:
							tile.setTexture(T_Cover_5);
							break;

						case 6:
							tile.setTexture(T_Cover_6);
							break;

						case 7:
							tile.setTexture(T_Cover_7);
							break;

						case 8:
							tile.setTexture(T_Cover_8);
							break;

						}

					}
					else {

						tile.setTexture(T_Uncover);

					}

					window.draw(tile);

				}

			}

		}

	}

	void updatePrompt() {

		std::cout << "Cover: " << std::endl; displayTile(tileCover);

		std::cout << "State: " << std::endl; displayTile(tileState);

		std::cout << "Check: " << std::endl; displayTile(tileCheck);

	}

	// Draw using draw voids that has been made
	void draw(sf::RenderWindow &window) {

		drawBackground(window);

		window.draw(game);

		drawGrid(window);

		// Panels
		flagPanel.setText(flag);
		flagPanel.draw(window);

		// Clock system
		if (timer) { gameTime = gameTimer.getElapsedTime().asSeconds(); }
		if (gameTime > 999) {
			if (999 * 60 <= gameTime) {

				gameTime = 999 * 60;

			}
			timePanel.setText(gameTime / 60);
		}
		else {
			timePanel.setText(gameTime);
		}
		timePanel.draw(window);

		B_Easy.draw(window);
		B_Medium.draw(window);
		B_Hard.draw(window);
		B_Brave.draw(window);
		B_IWarnedYou.draw(window);
		B_GetLost.draw(window);

	}

private:
	sf::RectangleShape game;

	sf::Sprite tile;
	sf::Sprite Background1r1;

	sf::Vertex line[2];

	sf::Clock gameTimer;

	sf::Sound sound;
	sf::SoundBuffer S_Uncover;
	sf::SoundBuffer S_Flag;
	sf::SoundBuffer S_Load;
	sf::SoundBuffer S_Win;
	sf::SoundBuffer S_Explode;
	sf::SoundBuffer S_Press;

	sf::Texture T_Bomb;
	sf::Texture T_Flag;
	sf::Texture T_Cover;
	sf::Texture T_Uncover;

	sf::Texture T_Cover_1;
	sf::Texture T_Cover_2;
	sf::Texture T_Cover_3;
	sf::Texture T_Cover_4;
	sf::Texture T_Cover_5;
	sf::Texture T_Cover_6;
	sf::Texture T_Cover_7;
	sf::Texture T_Cover_8;
	sf::Texture T_Cover_N[8];

	sf::Texture T_Background1r1;

	// Panels
	infoPanel flagPanel, timePanel;
	buttonPanel B_Easy, B_Medium, B_Hard, B_Brave, B_IWarnedYou, B_GetLost;

	// Game
	bool gameState = false;
	bool gameSound = true;
	bool timer = false;
	float gameX;
	float gameY;
	float gameOffsetX;
	float gameOffsetY;
	float gameWidth;
	float gameHeight;

	// Board
	int tileCover[200][200], tileState[200][200], tileCheck[200][200];
	int boardSize;
	int boardBomb;
	int flag;
	int gameTime = 0;
	int Lvl = 0; // [0 - Easy] [1 - Medium] [2 - Hard] [3 - Brave] [4 - I warned you] [5 - GetLost];
	int LvlSize[6][2] = { {10, 10}, {18, 35}, {28, 130}, {35, 210}, {50, 350}, {80, 999} };
	
	const int COVERED = 0, UNCOVERED = 1, FLAGGED = 2; // For tileCover

	const int CLEAR = 0, BOMB = 1; // For tileState

	const int UNCHECKED = 0, CHECKING = 1, CHECKED = 2; // For tileChecked

	// 0, 45, 90, 135, 180, 225, 270, 315
	const int direction[8][2] = { {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1,}, {0, -1}, {1, -1} };

	// Line
	sf::Color lineC = sf::Color::Black;

};

// This is self explanatory
class background {

	public:
		background(sf::Color Icolor) {

			shape.setFillColor(Icolor);
			shape.setPosition(sf::Vector2f(0.0f, 0.0f));
			shape.setSize(sf::Vector2f(window_Width, window_Height));

		}

		void setColor(sf::Color Icolor) {

			shape.setFillColor(Icolor);

		}

		void resize() {

			shape.setSize(sf::Vector2f(window_Width, window_Height));

		}

		void draw(sf::RenderWindow &window) {

			window.draw(shape);

		}

	private:
		sf::RectangleShape shape;

};

// Display some information about variables or anything else
void displayDebug() {

	std::cout << "[Window]" << std::endl;
	std::cout << "WindowSize: " << window_Size << std::endl;
	std::cout << "WindowScale: " << window_Scale << std::endl << std::endl;

}

// setup window by applying ratio and size
void setWindow(sf::RenderWindow &window) {

	float size;

	if (window_Fullscreen) {

		size = sf::VideoMode().getDesktopMode().width / window_Ratio[window_Type][0];

		window_Width = size * window_Ratio[window_Type][0];
		window_Height = size * window_Ratio[window_Type][1];

		window.create(sf::VideoMode(window_Width, window_Height), "MineSweeper " + window_Version, sf::Style::Fullscreen);

	}
	else {

		size = window_Size;

		window_Width = size * window_Ratio[window_Type][0];
		window_Height = size * window_Ratio[window_Type][1];

		window.create(sf::VideoMode(window_Width, window_Height), "MineSweeper " + window_Version, sf::Style::Close);

	}

	sf::Image icon;
	icon.loadFromFile("Style/Icon/Icon.jpg");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(60);

	window_Scale = (float) size / 75;

}

int main() {

	sf::RenderWindow window;
	setWindow(window);

	// Object
	MineSweeper MS(sf::Vector2f(window_Width / 2 + 25.0f, 25.0f), sf::Vector2f(window_Height - 50, window_Height - 50), sf::Vector2f(-window_Height / 2, 0.0f)); // GridSize: 10, BombAmount:10 are easy | GridSize: 18, BombAmount: 35 Normal
	background windowBackground(sf::Color::Color(155, 155, 155));

	while (window.isOpen()) {

		// Mouse position
		mouse_x = (float) sf::Mouse::getPosition(window).x;
		mouse_y = (float) sf::Mouse::getPosition(window).y;

		sf::Event evnt;
		while (window.pollEvent(evnt)) {

			switch (evnt.type) {

			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (KEY_SPACE) {

					window_Fullscreen = !window_Fullscreen;
					setWindow(window);

					// Resize
					windowBackground.resize();

					MS.resize();

					// Redraw
					window_Redraw = true;

				}
				if (KEY_C) {

					window.close();

				}
				if (KEY_R) {

					MS.restart();
					window_Redraw = true;

				}
				if (KEY_M) {

					MS.toggleSound();

				}
				break;

			case sf::Event::MouseButtonPressed:
				if (BTN_LEFT) {

					MS.uncoverTile(mouse_x, mouse_y);

					MS.setLevel();

					// Redraw
					window_Redraw = true;

				}
				if (BTN_RIGHT) {

					MS.flagTile(mouse_x, mouse_y);
					
					// Redraw
					window_Redraw = true;
				}
				break;
			}

		}
		
		windowBackground.draw(window);

		MS.draw(window);

		window.display();

		if (window_Redraw) {

			
			//system("cls"); MS.updatePrompt(); // Only for debugging purpose

			window_Redraw = false;

		}

	}

	return 0;

}