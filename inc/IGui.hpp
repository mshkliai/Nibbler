#ifndef IGUI_H
# define IGUI_H

# include <iostream>

class 					IGui
{
	public:
		
		virtual void	openWindow(int width, int height) = 0;
		virtual void 	closeWindow() = 0;
		virtual int 	getch() = 0;
		virtual void	drawMap() = 0;
		virtual void 	drawBody(int x, int y) = 0;
		virtual void	drawEnemyBody(int x, int y) = 0;
		virtual void	display(int speed) = 0;
		virtual void	drawGameOver() = 0;
		virtual void	drawNewGame() = 0;
		virtual void	drawToken(int x, int y) = 0;
		virtual void	drawToken(int x, int y, int) = 0;
		virtual void 	drawBonusToken(int x, int y) = 0;
		virtual void	drawSnakeHead(int x, int y) = 0;
		virtual void	drawEnemyHead(int x, int y) = 0;
		virtual void	drawPause() = 0;
		virtual void	drawScores(int scores, int highScore) = 0;
		virtual void	drawBarrier(int x, int y) = 0;
		virtual void	drawSplitter() = 0;
		virtual void	drawScoresMultiplayer(int scores, int enemyScores) = 0;
		virtual void	drawTimer(int timer) = 0;
		virtual void	drawWinner(std::string const &winner) = 0;
		virtual void	drawMenu(int cursor) = 0;
		virtual void 	playSound(int num) = 0;

		virtual	~IGui() { }
};

#endif