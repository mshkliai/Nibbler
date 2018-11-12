#include "SFMLGui.hpp"

SFMLGui::SFMLGui(int squareSize) : squareSize(squareSize), cadr(0), cadrToken(0)
 {
 	srand(time(NULL));

	this->textures.insert ( std::pair<std::string, sf::Texture>("map", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("header", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("body", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("token", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("headSnake", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("pause", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("barrier", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("splitter", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("headerMultiplayer", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("headEnemy", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("menu", sf::Texture()) );
	this->textures.insert ( std::pair<std::string, sf::Texture>("tokenEnemy", sf::Texture()) );

	this->sprites.insert ( std::pair<std::string, sf::Sprite>("map", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("header", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("body", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("token", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("headSnake", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("pause", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("barrier", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("splitter", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("headerMultiplayer", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("headEnemy", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("menu", sf::Sprite()) );
	this->sprites.insert ( std::pair<std::string, sf::Sprite>("tokenEnemy", sf::Sprite()) );

	this->init();
}

SFMLGui::SFMLGui(SFMLGui const &obj)
{
	*this = obj;
}

SFMLGui::~SFMLGui() { }

SFMLGui	&SFMLGui::operator = (SFMLGui const &)
{
	return *this;
}

void	SFMLGui::openWindow(int width, int height)
{
	sf::Vector2<int> position( (5120 - width) >> 1, (2880 - height) >> 1 );
	this->sizeMap[0] = width;
	this->sizeMap[1] = height;

	this->cX = this->sizeMap[0] / this->squareSize;
	this->cY = (this->sizeMap[1] / this->squareSize) - 2;

	this->renderWindow = new sf::RenderWindow(sf::VideoMode(width, height), "Nibbler SFML");
	this->renderWindow->setPosition(position);
}

 void 	SFMLGui::closeWindow()
 {
 	this->renderWindow->close();
 	delete this->renderWindow;
 }

int		SFMLGui::getch()
{
	while ( this->renderWindow->pollEvent(this->event) )
	{
		if (event.type == sf::Event::Closed)
			return -1;
		if ( event.type == sf::Event::KeyPressed )
		{
			if (event.key.code == 36)
				return 27;
			else if (event.key.code > 26 && event.key.code < 30)
				return event.key.code - 26;

			return event.key.code;
		}
	}

	return 0;
}

void 	SFMLGui::drawMap()
{
	this->sprites["map"].setPosition(0, this->squareSize << 1);

	for (int y = 0; y < this->cY; y++)
	{
		for (int x = 0; x < this->cX; x++)
		{
			this->renderWindow->draw(this->sprites["map"]);
			this->sprites["map"].move(this->squareSize, 0);
		}

		this->sprites["map"].move(-this->cX * this->squareSize, this->squareSize);
	}
}

void 	SFMLGui::drawBody(int x, int y)
{
	this->sprites["body"].setPosition(x, y);
	this->sprites["body"].setColor(sf::Color(0, 168, 107));

	this->renderWindow->draw(this->sprites["body"]);
}

void 	SFMLGui::drawEnemyBody(int x, int y)
{
	this->sprites["body"].setPosition(x, y);
	this->sprites["body"].setColor(sf::Color(205, 91, 69));

	this->renderWindow->draw(this->sprites["body"]);
}

void 	SFMLGui::display(int speed)
{
	sf::sleep(sf::milliseconds(speed));
	this->renderWindow->display();
}

void 	SFMLGui::drawGameOver()
{
	sf::Texture animationTexture;
	sf::Sprite 	animationSprite;

	animationTexture.loadFromFile( "./dynLib1/resources/gameOver/" + this->toStr(this->cadr++) + ".png", sf::Rect<int>(0, 0, 800, 800) );
	animationSprite.setTexture(animationTexture, true);
	animationSprite.setScale(this->sizeMap[0] / 800.0, this->sizeMap[1] / 800.0);

	this->renderWindow->draw(animationSprite);
	this->cadr == 45 ? this->cadr >>= 10 : 0;
}

/* Private Functions */

void 	SFMLGui::init()
{
	sf::Uint8 pixels[2][4] = { {0,0,0,255}, {255, 255, 255, 255} };

	this->menuFont.loadFromFile("./dynLib1/resources/fonts/menuFont.ttf");
	this->menuText.setFont(this->menuFont);

	this->textures["map"].loadFromFile("./dynLib1/resources/backgrounds/background.png", sf::Rect<int>(0, 0, this->squareSize, this->squareSize));
	this->textures["body"].loadFromFile("./dynLib1/resources/AnotherImages/snake.png", sf::Rect<int>(0, 0, this->squareSize, this->squareSize));
	this->textures["headSnake"].loadFromFile("./dynLib1/resources/AnotherImages/head.png", sf::Rect<int>(0, 0, 60, 60) );
	this->textures["pause"].loadFromFile("./dynLib1/resources/AnotherImages/pause.png", sf::Rect<int>(0, 0, 224, 224) );
	this->textures["header"].loadFromFile("./dynLib1/resources/backgrounds/background2.png", sf::Rect<int>(0, 0, 480, 300) );
	this->textures["barrier"].loadFromFile("./dynLib1/resources/AnotherImages/barrier.png", sf::Rect<int>(0, 0, 90, 60) );
	this->textures["splitter"].loadFromFile("./dynLib1/resources/AnotherImages/kust.png", sf::Rect<int>(0, 0, 76, 76));
	this->textures["headerMultiplayer"].loadFromFile("./dynLib1/resources/backgrounds/background3.png", sf::Rect<int>(0, 0, 480, 300) );
	this->textures["headEnemy"].loadFromFile("./dynLib1/resources/AnotherImages/headEnemy.png", sf::Rect<int>(0, 0, 80, 64) );
	this->textures["menu"].loadFromFile("./dynLib1/resources/backgrounds/menu.png", sf::Rect<int>(0, 0, 1800, 1800) );

	for (int i = 0; i < 60; i++)
	{
		if (i < this->squareSize)
		{
			this->textures["map"].update(pixels[0], 1, 1, i, 0);
			this->textures["map"].update(pixels[0], 1, 1, 0, i);

			this->textures["body"].update(pixels[1], 2, 2, i, 0);
			this->textures["body"].update(pixels[1], 2, 2, 0, i);
			this->textures["body"].update(pixels[1], 2, 2, this->squareSize - 2, i);
			this->textures["body"].update(pixels[1], 2, 2, i, this->squareSize - 2);
		}

		this->textures["headSnake"].update(pixels[1], 2, 2, i, 0);
		this->textures["headSnake"].update(pixels[1], 2, 2, 0, i);
		this->textures["headSnake"].update(pixels[1], 2, 2, 58, i);
		this->textures["headSnake"].update(pixels[1], 2, 2, i, 58);
	}

	this->sprites["header"].setTexture(this->textures["header"], true);
	this->sprites["map"].setTexture(this->textures["map"], true);
	this->sprites["body"].setTexture(this->textures["body"], true);
	this->sprites["headSnake"].setTexture(this->textures["headSnake"]);
	this->sprites["pause"].setTexture(this->textures["pause"]);
	this->sprites["barrier"].setTexture(this->textures["barrier"]);
	this->sprites["splitter"].setTexture(this->textures["splitter"]);
	this->sprites["headerMultiplayer"].setTexture(this->textures["headerMultiplayer"]);
	this->sprites["headEnemy"].setTexture(this->textures["headEnemy"]);
	this->sprites["menu"].setTexture(this->textures["menu"]);


	this->sprites["splitter"].setColor(sf::Color::Black);

	this->sprites["headSnake"].setScale(this->squareSize / 60.0, this->squareSize / 60.0);
	this->sprites["headEnemy"].setScale(this->squareSize / 80.0, this->squareSize / 64.0);
	this->sprites["barrier"].setScale(this->squareSize / 90.0, this->squareSize / 60.0);
}

void 	SFMLGui::drawNewGame()
{
	sf::Texture timerTexture;
	sf::Sprite 	timerSprite;

	for (int i = 0; i < 18 && this->getch() != 27; i++)
	{
		timerTexture.loadFromFile("./dynLib1/resources/loading/" + this->toStr(i) + ".png", sf::Rect<int>(0, 0, 800, 582));
		timerSprite.setTexture(timerTexture);
		timerSprite.setScale( (this->sizeMap[0]) / 800.0, (this->sizeMap[1]) / 582.0 );

		this->renderWindow->draw(timerSprite);
		this->renderWindow->display();
		sf::sleep(sf::milliseconds(70));
	}
}

void 	SFMLGui::drawToken(int x, int y)
{
	sf::Image	tokenImage;
	std::string fileName[6] = {"pig", "heart", "nice", "pear", "pig2", "bird"};

	if ( (this->sprites["token"].getPosition().x != x && this->sprites["token"].getPosition().y != y) ||
		 this->sprites["token"].getTexture() == NULL )
	{
		tokenImage.loadFromFile("./dynLib1/resources/Tokens/" + fileName[rand() % 6] + ".png");
		this->textures["token"].loadFromImage(tokenImage);
		this->sprites["token"].setTexture(this->textures["token"]);
		
		this->sprites["token"].setScale( static_cast<double>(this->squareSize) / static_cast<double>(this->textures["token"].getSize().x),
			static_cast<double>(this->squareSize) / static_cast<double>(this->textures["token"].getSize().y) );
	}
	this->sprites["token"].setPosition(x, y);
	this->renderWindow->draw(this->sprites["token"]);
}

void 	SFMLGui::drawToken(int x, int y, int)
{
	sf::Image	tokenImage;
	std::string fileName[6] = {"pig2", "pig", "pear", "nice", "bird", "heart"};

	if ( (this->sprites["tokenEnemy"].getPosition().x != x && this->sprites["tokenEnemy"].getPosition().y != y) ||
		this->sprites["token"].getTexture() == NULL )
	{
		tokenImage.loadFromFile("./dynLib1/resources/Tokens/" + fileName[rand() % 6] + ".png");
		this->textures["tokenEnemy"].loadFromImage(tokenImage);
		this->sprites["tokenEnemy"].setTexture(this->textures["tokenEnemy"]);
		
		this->sprites["tokenEnemy"].setScale( static_cast<double>(this->squareSize) / static_cast<double>(this->textures["tokenEnemy"].getSize().x),
			static_cast<double>(this->squareSize) / static_cast<double>(this->textures["tokenEnemy"].getSize().y) );
	}

	this->sprites["tokenEnemy"].setPosition(x, y);
	this->renderWindow->draw(this->sprites["tokenEnemy"]);
}

void 	SFMLGui::drawBonusToken(int x, int y)
{
	sf::Texture tokenTexture;
	sf::Sprite 	tokenSprite;

	tokenTexture.loadFromFile("./dynLib1/resources/BonusToken/" + this->toStr(this->cadrToken++) + ".png", sf::Rect<int>(0, 0, 60, 60));
	tokenSprite.setTexture(tokenTexture);
	tokenSprite.setScale(this->squareSize / 60.0, this->squareSize / 60.0);
	tokenSprite.setPosition(x, y);
	this->renderWindow->draw(tokenSprite);

	this->cadrToken > 28 ? this->cadrToken >>= 10 : 0;
}

void 	SFMLGui::drawSnakeHead(int x, int y)
{
	this->sprites["headSnake"].setPosition(x, y);
	this->renderWindow->draw(this->sprites["headSnake"]);
}

void 	SFMLGui::drawEnemyHead(int x, int y)
{
	this->sprites["headEnemy"].setPosition(x, y);
	this->renderWindow->draw(this->sprites["headEnemy"]);
}

void 	SFMLGui::drawPause()
{
	this->sprites["pause"].setPosition( (this->sizeMap[0] - 224) >> 1, (this->sizeMap[1] - 224) >> 1 );

	this->renderWindow->draw(this->sprites["pause"]);
	this->renderWindow->display();
}

void 	SFMLGui::drawScores(int scores, int highScore)
{
	this->font.loadFromFile("./dynLib1/resources/fonts/fontScores.ttf");
	this->text.setFont(this->font);

	this->text.setCharacterSize((this->squareSize << 1) - 20);
	this->text.setFillColor(sf::Color::Cyan);
	this->text.setPosition(5, -5);
	this->sprites["header"].setScale(this->sizeMap[0] / 460.0, (this->squareSize << 1) / 300.0);
	this->text.setString("Scores:  " + this->toStr(scores));

	this->renderWindow->draw(this->sprites["header"]);
	this->renderWindow->draw(this->text);

	this->text.setString("High Score:  " + this->toStr(highScore));

	if (scores != highScore)
	{
		this->text.setPosition(this->sizeMap[0] / 3, 15);
		this->text.setCharacterSize(this->squareSize);
		this->text.setFillColor(sf::Color::Yellow);
	}
	else
	{
		this->text.setPosition(this->sizeMap[0] / 3, 0);
		this->text.setCharacterSize( (this->squareSize << 1) - 30);
	}

	this->renderWindow->draw(this->text);
}

void 	SFMLGui::drawBarrier(int x, int y)
{
	this->sprites["barrier"].setPosition(x, y);
	this->renderWindow->draw(this->sprites["barrier"]);
}

void 	SFMLGui::drawSplitter()
{
	this->sprites["splitter"].setPosition( ((this->cX + 1) >> 1) * this->squareSize, this->squareSize << 1);
	this->sprites["splitter"].setScale(this->squareSize / 76.0, this->squareSize / 76.0);

	for (int i = 0; i < this->cY; i++)
	{
		this->renderWindow->draw(this->sprites["splitter"]);
		this->sprites["splitter"].move(0, this->squareSize);
	}
}

void 	SFMLGui::drawScoresMultiplayer(int scores, int enemyScores)
{
	this->font.loadFromFile("./dynLib1/resources/fonts/fontScores.ttf");
	this->text.setFont(this->font);

	this->sprites["headerMultiplayer"].setScale(this->sizeMap[0] / 460.0, (this->squareSize << 1) / 300.0);
	this->renderWindow->draw(this->sprites["headerMultiplayer"]);

	this->text.setCharacterSize((this->squareSize << 1) - 20);
	this->text.setFillColor( sf::Color::Color(141, 182, 0) );
	this->text.setPosition(100, -5);
	this->text.setString("You:  " + this->toStr(scores));
	this->renderWindow->draw(this->text);

	this->text.setFillColor( sf::Color::Color(205, 91, 69) );
	this->text.setPosition(this->sizeMap[0] - (this->sizeMap[0] >> 2), -3);
	this->text.setString("Bot:  " + this->toStr(enemyScores));
	this->renderWindow->draw(this->text);
}

void 	SFMLGui::drawTimer(int timer)
{
	time_t		t = static_cast<time_t>(60 - timer);
	struct tm	*time_stamp = gmtime(&t);
	char		buff[6];

	time_stamp = gmtime(&t);
	strftime(buff, 6, "%M:%S\0", time_stamp);

	this->font.loadFromFile("./dynLib1/resources/fonts/fontTimer.ttf");
	this->text.setFont(this->font);
	this->text.setPosition( (this->sizeMap[0] >> 1) - (this->squareSize << 1), 3 );
	this->text.setCharacterSize((this->squareSize << 1) - 20);
	this->text.setFillColor(60 - timer <= 5 ? sf::Color::Red : sf::Color::Black);

	this->text.setString(buff);
	this->renderWindow->draw(this->text);
}

void 	SFMLGui::drawWinner(std::string const &winner)
{
	if (this->text.getFont() == NULL)
	{
		this->font.loadFromFile("./dynLib1/resources/fonts/fontTimer.ttf");
		this->text.setFont(this->font);
	}
	this->text.setCharacterSize(this->sizeMap[0] >> 3);
	this->text.setString("Winner: " + winner);
	this->text.setFillColor(sf::Color::Black);
	this->text.setPosition( (this->sizeMap[0] >> 1) - (this->sizeMap[0] >> 2), (this->sizeMap[1] >> 1) - (this->sizeMap[1] >> 3));

	this->renderWindow->draw(this->text);
}

void 	SFMLGui::drawMenu(int cursor)
{
	this->sprites["menu"].setScale(this->sizeMap[0] / 1800.0, this->sizeMap[1] / 1800.0);
	this->renderWindow->draw(this->sprites["menu"]);

	this->menuText.setCharacterSize(this->sizeMap[0] >> 5);
	this->menuText.setPosition( (this->sizeMap[0] >> 1) - (this->sizeMap[0] >> 3), this->sizeMap[1] >> 2);
	this->menuText.setString("New game");
	this->menuText.setFillColor(cursor == 0 ? sf::Color(59, 68, 75) : sf::Color::White);

	this->renderWindow->draw(this->menuText);

	this->menuText.setPosition( (this->sizeMap[0] >> 1) - (this->sizeMap[0] >> 3) - (this->sizeMap[0] >> 7), (this->sizeMap[1] >> 2) + (this->sizeMap[0] >> 4) );
	this->menuText.setString("Play vs bot");
	this->menuText.setFillColor(cursor == 1 ? sf::Color(59, 68, 75) : sf::Color::White);

	this->renderWindow->draw(this->menuText);

	this->menuText.setPosition( (this->sizeMap[0] >> 2) + (this->sizeMap[0] >> 3) + (this->sizeMap[0] >> 5), (this->sizeMap[1] >> 2) + (this->sizeMap[0] >> 3) );
	this->menuText.setString("Exit");
	this->menuText.setFillColor(cursor == 2 ? sf::Color(59, 68, 75) : sf::Color::White);

	this->renderWindow->draw(this->menuText);
}


void	SFMLGui::playSound(int num)
{
	switch (num)
	{
		case 0:  system("afplay ./dynLib1/resources/sounds/menuCursor.wav &"); break;
		case 1:  system("afplay ./dynLib1/resources/sounds/menuEnter.wav &");  break;
		case 2:  system("afplay ./dynLib1/resources/sounds/token.wav &");      break;
		case 3:  system("afplay ./dynLib1/resources/sounds/enemyToken.wav &"); break;
		default: system("afplay ./dynLib1/resources/sounds/gameOver.wav &");
	}
}

/************************Private************************/

std::string 	SFMLGui::toStr(int num)
{
	std::stringstream 	ss;
	ss << num;

	return ss.str();
}

/* C Functions */

extern "C" IGui* getGui(int squareSize)
{
	return new SFMLGui(squareSize);
}