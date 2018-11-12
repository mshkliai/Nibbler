#ifndef OPENGLGUI_H
# define OPENGLGUI_H

# include "../inc/IGui.hpp"

# include <freetype2/ft2build.h>
# include <SOIL.h>
# include <FTGL/ftgl.h>
# include <GLFW/glfw3.h>
# include <iostream>
# include <sstream>
# include <chrono>
# include <thread>
# include <map>

class OpenGLGui : public IGui {

	public:

		OpenGLGui();
		OpenGLGui(int squareSize);
		OpenGLGui(OpenGLGui const &obj);
		~OpenGLGui();

		OpenGLGui 			&operator = (OpenGLGui const &);

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

		int					count;
		int					wcount;
		int					hcount;
		int					start;
		int 				cadr;
		int 				cadrToken;
		int 				cadrWater;
		int 				token_x;
		int					token_y;
		int					token_n;
		int 				enemyToken_x;
		int					enemyToken_y;
		int					enemyToken_n;

		GLFWwindow			*window;

		FTGLPixmapFont		*font;

		std::string			fruits[5];
		GLuint				menuTexture[3];
		GLuint				bonusTexture[6];
		GLuint				gemTexture[5];
		GLuint				endTexture[6];
		GLuint				loadingTexture[6];

		std::map<std::string, GLuint>	gameTextures;

		int					sizeMap[2];
		int 				squareSize;

		void				init();
		void				init_rest();
		void				init_end();
		std::string 		toStr(int num);

};

#endif
