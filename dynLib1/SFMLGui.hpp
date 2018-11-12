#ifndef SFMLGUI_H
# define SFMLGUI_H

# include "../inc/IGui.hpp"

# include <SFML/Graphics.hpp>

# include <iostream>
# include <sstream>
# include <map>
# include <time.h>

class 										SFMLGui : public IGui
{

	public:

		SFMLGui();
		SFMLGui(int squareSize);
		SFMLGui(SFMLGui const &obj);
		~SFMLGui();

		SFMLGui 							&operator = (SFMLGui const &);

		void								openWindow(int width, int heigth);
		void 								closeWindow();

		int									getch();

		void								drawMap();
		void								drawBody(int x, int y);
		void								drawEnemyBody(int x, int y);
		void								display(int speed);
		void								drawGameOver();
		void								drawNewGame();
		void								drawToken(int x, int y);
		void								drawToken(int x, int y, int);
		void								drawBonusToken(int x, int y);
		void								drawSnakeHead(int x, int y);
		void								drawEnemyHead(int x, int y);
		void								drawPause();
		void								drawScores(int scores, int highScore);
		void								drawBarrier(int x, int y);
		void								drawSplitter();
		void								drawScoresMultiplayer(int scores, int enemyScores);
		void								drawTimer(int timer);
		void								drawWinner(std::string const &winner);
		void								drawMenu(int cursor);
		void								playSound(int num);

	private:

		sf::RenderWindow					*renderWindow;
		sf::Event							event;

		sf::Font							font;
		sf::Font 							menuFont;

		sf::Text 							menuText;
		sf::Text 							text;

		std::map<std::string, sf::Texture>	textures;
		std::map<std::string, sf::Sprite> 	sprites;

		std::vector<sf::Texture> 			gameOverTexture;
		std::vector<sf::Sprite> 			gameOverSprite;

		int 								cX;
		int 								cY;

		int									sizeMap[2];
		int 								squareSize;
		int 								cadr;
		int 								cadrToken;

		void								init();
		std::string 						toStr(int num);

};

#endif
