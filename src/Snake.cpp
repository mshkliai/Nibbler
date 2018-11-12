#include "../inc/Snake.hpp"

/************* Overloading operators of structure *************/

bool	t_cord::operator == (t_cord const &cords)
{
	return (this->x == cords.x && this->y == cords.y);
}

void	t_cord::operator = (t_cord const &cords)
{
	this->x = cords.x;
	this->y = cords.y;
}

/************* Class *************/

Snake::Snake(t_cord const &parameters, int blockSize) : mapSize(parameters), blockSize(blockSize), direction(right), scores(0),speed(70), blink(11), isGameOver(false), timer(time(NULL))
{
	srand( time(NULL) );

	this->bonusToken.x = -1;
	this->handleTime.x = -1;
	this->cX = ((this->mapSize.x / this->blockSize + 1) >> 1) * this->blockSize;

	this->getHighScore();
	this->dropToken(&(this->token));
	this->initSnake();
}

Snake::Snake(Snake const &obj)
{
	*this = obj;
}

Snake::~Snake() { }

Snake 	&Snake::operator = (Snake const &)
{
	return *this;
}

/************* Game *************/

int	Snake::startGame()
{
	int 	button;
	int 	newGameTimer = time(NULL);

	gui->drawNewGame();

	this->timer -= newGameTimer - time(NULL);

	while ( ( button = this->handleButtons() ) == 0 || button > 3 )
	{
		if (this->isGameOver == true)
		{
			this->GameOverPainter();

			if (button == 4)
				this->newGame();
		}
		else
		{
			this->mapPainter();
			this->moveSnake();
			this->drawSnake();
			this->headPainter();
			this->manageBarrier();
			this->checkCollision();
			this->manageTokens();
		}
		gui->display(this->speed);
	}
	this->saveHighScore();

	return button >= 1 && button <= 3 ? button : 0;
}

/************* Setters *************/

void	Snake::setGui(IGui *gui)
{
	this->gui = gui;
}

void	Snake::setLibsInfo(int countLibs, int numberOfLib)
{
	this->countLibs = countLibs;
	this->numberOfLib = numberOfLib;
}

/************* Score Management *************/

void	Snake::getHighScore()
{
	std::ifstream	a("score.txt");

	if (a.is_open() == false)
		this->highScore = 0;
	else
	{
		char	buff[8];
		a.getline(buff, 8);
		a.close();
		this->highScore = atoi(buff);
	}
}

void	Snake::saveHighScore()
{
	if (this->scores == this->highScore)
	{
		std::ofstream write;
		write.open("score.txt", std::ios::trunc);
		write << this->scores;
		write.close();
		this->highScore = this->scores;
	}
}

/************* Game Objects Managers *************/

void	Snake::manageTokens()
{
	if (this->handleTime.x == -1)
	{
		this->handleTime.x = time(NULL);
		this->handleTime.y = rand() % 8 + 1;
	}
	else
	{
		if (this->handleTime.x + this->handleTime.y <= time(NULL) && this->bonusToken.x == -1)
		{
			while (this->bonusToken.x == -1 || this->bonusToken.x == this->cX)
				dropToken(&(this->bonusToken));
			this->handleTime.x = time(NULL);
		}
		else if (this->bonusToken.x != -1 && this->handleTime.x + 3 < time(NULL))
		{
			this->handleTime.x = -1;
			this->bonusToken.x = -1;
		}
	}

	if (this->bonusToken.x != -1)
	{
		if (this->handleTime.x + 1 < time(NULL))
			this->blink++ > 10 ? this->blink >>= 5 : 0;

		if (this->blink > 5)
			gui->drawBonusToken(this->bonusToken.x, this->bonusToken.y);
	}
	gui->drawToken(this->token.x, this->token.y);
}

void	Snake::manageBarrier()
{
	while ( this->barrier.size() == 0 && this->scores > rand() % 300 + 50 )
		this->dropBarrier();

	for (std::vector<t_cord>::iterator i = this->barrier.begin(); i < this->barrier.end(); i++)
		gui->drawBarrier(i->x, i->y);
}

/************* Snake Managers *************/

void	Snake::growSnake()
{
	t_cord	newPartSnake = this->snake[0];

	this->snake.insert(this->snake.begin(), newPartSnake);

	this->speed > 40 ? this->speed -= 1 : 0;
}

void	Snake::initSnake()
{
	t_cord		square;

	this->snake.clear();
	this->snake.reserve(4);
	for (int i = 0; i < 4; i++)
	{
		square.x = ( (this->mapSize.x >> 1) + i * this->blockSize ) - (this->blockSize << 1);
		square.y = ( this->mapSize.y >> 1 ) - this->blockSize;
		square.x -= square.x % this->blockSize;
		square.y -= square.y % this->blockSize;

		this->snake.push_back(square);
	}
}

void	Snake::moveSnake()
{
	for (std::vector<t_cord>::iterator i = this->snake.begin(); i < (this->snake.end() - 1); i++)
	{
		i->x = (i + 1)->x;
		i->y = (i + 1)->y;
	}

	switch (direction)
	{
		case up:   (--this->snake.end())->y -= this->blockSize; break;
		case down: (--this->snake.end())->y += this->blockSize; break;
		case left: (--this->snake.end())->x -= this->blockSize; break;
		default:   (--this->snake.end())->x += this->blockSize;
	}
}

/************* Collisions *************/

void	Snake::checkCollision()
{
	if ( this->handleScope( *(--this->snake.end()) ) == false ||
		 std::find( this->snake.begin(), this->snake.end() - 2, *(--this->snake.end()) ) != this->snake.end() - 2 ||
		 std::find( this->barrier.begin(), this->barrier.end(), *(--this->snake.end()) ) != this->barrier.end() )
	{
		gui->playSound(4);
		this->isGameOver = true;
	}

	if ( *(--this->snake.end()) == this->token )
	{
		this->dropToken(&(this->token));
		this->growSnake();
		this->scores += 10;
		gui->playSound(2);
	}

	if ( *(--this->snake.end()) == this->bonusToken )
	{
		this->bonusToken.x = -1;
		this->handleTime.x = -1;
		this->scores += (rand() % 95) + 5;
		gui->playSound(2);
	}

	this->highScore < this->scores ? this->highScore = this->scores : 0;
}

bool	Snake::handleScope(t_cord const &object)
{
	if ( object.x >= this->mapSize.x || object.y >= this->mapSize.y ||
		 object.x < 0 || object.y < (this->blockSize << 1) )
	{
		return false;
	}
	return true;
}

/************* Painters *************/

void	Snake::newGame()
{
	this->saveHighScore();
	this->direction = right;
	this->isGameOver = false;

	this->barrier.clear();

	this->initSnake();
	this->dropToken(&(this->token));
	this->scores = 0;
	this->speed = 70;
	this->bonusToken.x = -1;
	this->blink = 11;

	gui->drawNewGame();
}

void	Snake::mapPainter()
{
	gui->drawMap();
}

void	Snake::headPainter()
{
	gui->drawScores(this->scores, this->highScore);
}

void	Snake::GameOverPainter()
{
	gui->drawGameOver();
}

void	Snake::drawSnake()
{
	for (std::vector<t_cord>::iterator i = this->snake.begin(); i < (this->snake.end() - 1); i++)
		gui->drawBody(i->x, i->y);

	gui->drawSnakeHead( ( --this->snake.end() )->x, ( --this->snake.end() )->y );
}

void	Snake::dropToken(t_cord *token)
{
	token->x = rand() % this->mapSize.x;
	token->y = rand() % (this->mapSize.y - (this->blockSize << 1)) + (this->blockSize << 1);

	token->x -= token->x % this->blockSize;
	token->y -= token->y % this->blockSize;

	if ( std::find(this->snake.begin(), this->snake.end(), *token) != this->snake.end() ||
	  	 std::find(this->barrier.begin(), this->barrier.end(), *token) != this->barrier.end() )
	{
		this->dropToken(token);
	}
}

/************* Private Methods *************/

/************* Parsers *************/



int 	Snake::handleButtons()
{
	switch ( gui->getch() )
	{
		case up:	this->direction != down ? this->direction = up : 0;     break;
		case down:	this->direction != up ? this->direction = down : 0;     break;
		case left:	this->direction != right ? this->direction = left : 0;  break;
		case right:	this->direction != left ? this->direction = right : 0;  break;

		case esc:	return -1;
		case space:	return 4;
		case Pause:	return this->pause();
		case 1:		return this->numberOfLib != 1 ? 1 : 0;
		case 2:		return this->numberOfLib != 2 && this->countLibs > 1 ? 2 : 0;
		case 3:		return this->numberOfLib != 3 && this->countLibs > 2 ? 3 : 0;
		default:	break;
	}
	return 0;
}

/************* Another Methods *************/

void	Snake::dropBarrier()
{
	int 	size = rand() % 10 + 5;

	t_cord	newBarr[size];

	newBarr[0].x = rand() % this->mapSize.x;
	newBarr[0].y = rand() % this->mapSize.y;

	newBarr[0].x -= newBarr[0].x % this->blockSize;
	newBarr[0].y -= newBarr[0].y % this->blockSize;

	if ( std::find(this->snake.begin(), this->snake.end(), newBarr[0]) != this->snake.end() )
		return ;

	for (int i = 1; i < size; i++)
	{
		newBarr[i] = newBarr[i - 1];

		switch ( rand() % 4 )
		{
			case 1:  newBarr[i].x += this->blockSize; break;
			case 2:  newBarr[i].x -= this->blockSize; break;
			case 3:  newBarr[i].y += this->blockSize; break;
			default: newBarr[i].y -= this->blockSize;
		}

		for (int j = 0; j < i; j++)
		{
			if (newBarr[j] == newBarr[i])
				return ;
		}

		if ( std::find(this->snake.begin(), this->snake.end(), newBarr[i]) != this->snake.end()
			|| this->handleScope(newBarr[i]) == false )
		{
			return ;
		}
	}

	for (int i = 0; i < size; i++)
		this->barrier.push_back(newBarr[i]);
}

int		Snake::pause()
{
	int button = 0;
	int timerPause = time(NULL);

	gui->drawPause();

	while (button != Pause && button != esc && !(button >= 1 && button <= 3))
		button = gui->getch();

	this->timer -= timerPause - time(NULL);
	if (button >= 1 && button <= 3)
		return button;
	return button == esc ? -1 : 0;
}
