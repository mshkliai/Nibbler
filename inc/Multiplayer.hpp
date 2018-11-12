# ifndef MULTIPLAYER_H
# define MULTIPLAYER_H

# include "includes.h"
# include "IGui.hpp"
# include "Snake.hpp"

class 	Multiplayer : public Snake
{

	public:
	
		Multiplayer();
		Multiplayer(t_cord const &parameters, int blockSize);
		Multiplayer(Multiplayer const &obj);
		~Multiplayer();

		Multiplayer &operator = (Multiplayer const &);

	private:

/************* Score Management *************/

		void				getHighScore();
		void				saveHighScore();

/************* Snake Managers *************/

		void				growSnake();
		void				initSnake();
		void				drawSnake();
		void				moveSnake();

/************* Checkers and Collisions *************/

		void				checkTime();
		void				checkCollision();
		bool				handleScope(t_cord const &object);

/************* Game Objects Managers *************/

		void				manageTokens();
		void				manageBarrier();

/************* Painters *************/

		void				newGame();
		void				mapPainter();
		void				headPainter();
		void				GameOverPainter();

/************* Another Game Process Methods *************/

		void				dropEnemyToken();
		void				niceDirectionEnemy();

/************* Game Variables *************/

		t_cord				enemyToken;
		std::vector<t_cord> enemySnake;

		int 				enemyDirection;
		int 				enemyScores;

};

#endif