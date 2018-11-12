#ifndef SDL2GUI_H
# define SDL2GUI_H

# include "../inc/IGui.hpp"

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <iostream>
# include <sstream>
# include <map>

class SDL2Gui : public IGui {

	public:

		SDL2Gui();
		SDL2Gui(int squareSize);
		SDL2Gui(SDL2Gui const &obj);
		~SDL2Gui();

		SDL2Gui 			&operator = (SDL2Gui const &);

		void				openWindow(int width, int heigth);
		void 				closeWindow();
		int					getch();
		void				drawMap();
		void				drawBody(int x, int y);
		void				drawEnemyBody(int x, int y);
		void				display(int speed);
		void				drawGameOver();
		void				drawNewGame();
		void				drawToken(int x, int y);
		void				drawToken(int x, int y, int);
		void				drawBonusToken(int x, int y);
		void				drawSnakeHead(int x, int y);
		void				drawEnemyHead(int x, int y);
		void				drawPause();
		void				drawScores(int scores, int highScore);
		void				drawBarrier(int x, int y);
		void				drawSplitter();
		void				drawScoresMultiplayer(int scores, int enemyScores);
		void				drawTimer(int timer);
		void				drawWinner(std::string const &winner);
		void				drawMenu(int cursor);
		void				playSound(int num);

	private:

		SDL_Window			*renderWindow;
		SDL_Renderer		*renderer;

		int					count;
		int					wcount;
		int					hcount;
		int 				cadr;
		int 				cadrToken;
		int 				cadrWater;
		int 				token_x;
		int					token_y;
		int					token_n;
		int 				enemyToken_x;
		int					enemyToken_y;
		int					enemyToken_n;

		std::string			menuFont;

		std::map<std::string, SDL_Texture *> gameTextures;
		SDL_Texture			*loadingTexture[6];
		SDL_Texture			*tokenTexture[5];
		SDL_Texture			*bonusTexture[50];
		SDL_Texture			*gameOverTexture[10];
		SDL_Texture			*waterTexture[58];

		int					sizeMap[2];
		int 				squareSize;

		void				init();
		std::string 		toStr(int num);

};

#endif
