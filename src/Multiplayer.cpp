#include "../inc/Multiplayer.hpp"

Multiplayer::Multiplayer(t_cord const &parameters, int blockSize) : Snake(parameters, blockSize), enemyDirection(left), enemyScores(0)
{
	this->initSnake();
	this->dropEnemyToken();
}

Multiplayer::Multiplayer(Multiplayer const &obj) : Snake(obj) { }

Multiplayer::~Multiplayer() { }

Multiplayer	&Multiplayer::operator = (Multiplayer const &)
{
	return *this;
}

/************* Score Management *************/

void		Multiplayer::getHighScore()  { }
void		Multiplayer::saveHighScore() { }

/************* Snake Managers *************/

void		Multiplayer::growSnake()
{
	this->speed > 40 ? this->speed -= 2 : 0;
}

void		Multiplayer::initSnake()
{
	Snake::initSnake();

	for (std::vector<t_cord>::iterator i = this->snake.begin(); i < this->snake.end(); i++)
		i->x -= (this->blockSize << 4);

	t_cord	square;

	this->enemySnake.clear();
	this->enemySnake.reserve(4);
	for (int i = 0; i < 4; i++)
	{
		square.x = ( (this->mapSize.x >> 1) - i * this->blockSize ) + (this->blockSize << 4);
		square.y = ( this->mapSize.y >> 1 ) - this->blockSize;
		square.x -= square.x % this->blockSize;
		square.y -= square.y % this->blockSize;

		this->enemySnake.push_back(square);
	}
}

void		Multiplayer::drawSnake()
{
	Snake::drawSnake();

	for (std::vector<t_cord>::iterator i = this->enemySnake.begin(); i < (this->enemySnake.end() - 1); i++)
		gui->drawEnemyBody(i->x, i->y);

	gui->drawEnemyHead( ( --this->enemySnake.end() )->x, ( --this->enemySnake.end() )->y );
}

void		Multiplayer::moveSnake()
{
	Snake::moveSnake();
	this->niceDirectionEnemy();

	for (std::vector<t_cord>::iterator i = this->enemySnake.begin(); i < (this->enemySnake.end() - 1); i++)
	{
		i->x = (i + 1)->x;
		i->y = (i + 1)->y;
	}
	switch (this->enemyDirection)
	{
		case left:  ( --this->enemySnake.end() )->x -= this->blockSize;  break;
		case right: ( --this->enemySnake.end() )->x += this->blockSize;  break;
		case down:  ( --this->enemySnake.end() )->y += this->blockSize;  break;
		default:    ( --this->enemySnake.end() )->y -= this->blockSize;
	}
}

/************* Checkers and Collisions *************/

void		Multiplayer::checkTime()
{
	this->timer + 60 <= time(NULL) ? this->isGameOver = true : 0;
}

void		Multiplayer::checkCollision()
{
	Snake::checkCollision();

	this->checkTime();

	while (this->token.x >= this->mapSize.x >> 1)
		this->dropToken( &(this->token) );

	if ( *(--this->enemySnake.end() ) == this->enemyToken )
	{
		this->dropEnemyToken();
		this->enemyScores += 10;
		gui->playSound(3);
	}
	if ( *(--this->enemySnake.end()) == this->bonusToken )
	{
		this->bonusToken.x = -1;
		this->handleTime.x = -1;
		this->enemyScores += (rand() % 95) + 5;
		gui->playSound(3);
	}
}

bool		Multiplayer::handleScope(t_cord const &object)
{
	if ( Snake::handleScope(object) == false || object.x >= this->cX )
		return false;

	return true;
}

/************* Game Objects Managers *************/

void		Multiplayer::manageTokens()
{
	while ( std::find(this->enemySnake.begin(), this->enemySnake.end(), this->bonusToken) != this->enemySnake.end() )
		this->dropToken( &(this->bonusToken) );

	Snake::manageTokens();

	gui->drawToken(this->enemyToken.x, this->enemyToken.y, 1);
}

void		Multiplayer::manageBarrier() { }

/************* Painters *************/

void		Multiplayer::newGame()
{
	Snake::newGame();

	this->enemyDirection = left;
	this->timer = time(NULL);
	this->enemyScores = 0;
}

void		Multiplayer::mapPainter()
{
	Snake::mapPainter();

	gui->drawSplitter();
}

void		Multiplayer::headPainter()
{
	gui->drawScoresMultiplayer(this->scores, this->enemyScores);
	gui->drawTimer(time(NULL) - this->timer);
}

void		Multiplayer::GameOverPainter()
{
	Snake::GameOverPainter();

	gui->drawWinner(this->scores >= this->enemyScores ? "You" : "Bot");
}

/************* Another Game Process Methods *************/

void		Multiplayer::dropEnemyToken()
{
	this->enemyToken.x = -1;

	while ( this->enemyToken.x <= this->cX )
		this->dropToken( &(this->enemyToken) );

	if ( std::find(this->enemySnake.begin(), this->enemySnake.end(), this->enemyToken) != this->enemySnake.end() )
		this->dropEnemyToken();
}

void		Multiplayer::niceDirectionEnemy()
{
	t_cord &priorityToken = this->bonusToken.x > this->mapSize.x >> 1 ? this->bonusToken : this->enemyToken;

	if ( (--this->enemySnake.end() )->x < priorityToken.x && this->enemyDirection != left)
		this->enemyDirection = right;
	else if ( (--this->enemySnake.end() )->x > priorityToken.x && this->enemyDirection != right)
		this->enemyDirection = left;
	else if ( (--this->enemySnake.end() )->y > priorityToken.y && this->enemyDirection != down)
		this->enemyDirection = up;
	else if ( (--this->enemySnake.end() )->y < priorityToken.y && this->enemyDirection != up)
		this->enemyDirection = down;
	else if ( this->enemyDirection == left && ( --this->enemySnake.end() )->x - this->blockSize >= this->cX )
		this->enemyDirection = up;
	else if ( this->enemyDirection == right && ( --this->enemySnake.end() )->x + this->blockSize <= this->mapSize.x)
		this->enemyDirection = down;
	else if ( this->enemyDirection == down && ( --this->enemySnake.end() )->y + this->blockSize <= this->mapSize.y)
		this->enemyDirection = left;
	else if ( this->enemyDirection == up && ( --this->enemySnake.end() )->y - this->blockSize >= this->blockSize)
		this->enemyDirection = right;
}
