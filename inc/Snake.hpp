# ifndef SNAKE_H
# define SNAKE_H

# include "includes.h"
# include "IGui.hpp"

class 	Snake
{
	public:
		
		Snake();
		Snake(t_cord const &parameters, int blockSize);
		Snake(Snake const &obj);
		virtual ~Snake();

		Snake &operator = (Snake const &);

/************* Game *************/

		int		startGame();

/************* Setters *************/

		void	setGui(IGui *gui);
		void	setLibsInfo(int countLibs, int numberOfLib);
	
	protected:

/************* Gui Variables *************/

		IGui				*gui;

/************* Game Variables *************/

		t_cord 				mapSize;
		t_cord 				token;
		t_cord				bonusToken;
		t_cord 				handleTime;

		int					blockSize;
		int					direction;
		int 				scores;
		int 				highScore;
		int 				speed;
		int 				blink;

		int 				countLibs;
		int 				numberOfLib;

		bool				isGameOver;

		int 				timer;
		int 				cX; 
	

		std::vector<t_cord>	snake;
		std::vector<t_cord>	barrier;
	
/************* Score Management *************/

		virtual void	getHighScore();
		virtual void	saveHighScore();

/************* Game Objects Managers *************/
		
		virtual void	manageTokens();
		virtual void	manageBarrier();
		
/************* Snake Managers *************/
		
		virtual void	growSnake();
		virtual void	initSnake();
		virtual void	drawSnake();
		virtual void	moveSnake();

/************* Collisions *************/
		
		virtual void	checkCollision();
		virtual bool	handleScope(t_cord const &object);

/************* Painters *************/
		
		virtual void	newGame();
		virtual void	mapPainter();
		virtual void	headPainter();
		virtual void 	GameOverPainter();
		
		void			dropToken(t_cord *token);

	private:		

/************* Parsers *************/	
		
		int 			handleButtons();

/************* Another Methods *************/
	
		void			dropBarrier();
		int				pause();
};

#endif