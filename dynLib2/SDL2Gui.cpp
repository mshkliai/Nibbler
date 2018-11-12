#include "SDL2Gui.hpp"

SDL2Gui::SDL2Gui(int squareSize) : squareSize(squareSize) {
	if (SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
		return;
	}

	TTF_Init();
}

SDL2Gui::SDL2Gui(SDL2Gui const &obj) {
	*this = obj;
}

SDL2Gui::~SDL2Gui() { }

SDL2Gui	&SDL2Gui::operator = (SDL2Gui const &) {
	return *this;
}

void	SDL2Gui::openWindow(int width, int height) {

	this->sizeMap[0] = width;
	this->sizeMap[1] = height;
	this->renderWindow = SDL_CreateWindow("Nibbler SDL2", (5120 - width) >> 2, (2880 - height) >> 2, this->sizeMap[0] >> 1, this->sizeMap[1] >> 1, SDL_WINDOW_SHOWN);

	this->wcount = width / this->squareSize;
	this->hcount = height / this->squareSize;
	this->count = this->hcount * this->wcount;

	if (this->renderWindow == nullptr) {
		std::cout << "Failed to create window : " << SDL_GetError();
		return;
	}

	this->renderer = SDL_CreateRenderer(this->renderWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return;
	}
	this->init();
	SDL_RenderSetLogicalSize(this->renderer, this->sizeMap[0] >> 1, this->sizeMap[1] >> 1);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);

}

void 	SDL2Gui::closeWindow() {

	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->renderWindow);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int		SDL2Gui::getch() {
	SDL_Event e;
	for (;;) {
		SDL_PollEvent(&e);
		switch(e.type) {
			case SDL_QUIT:
				return 27;
			case SDL_KEYDOWN: {
				switch (e.key.keysym.sym) {
					case SDLK_RIGHT:
						return 72;
					case SDLK_LEFT:
						return 71;
					case SDLK_UP:
						return 73;
					case SDLK_DOWN:
						return 74;
					case SDLK_SPACE:
						return 57;
					case SDLK_p:
						return 15;
					case SDLK_ESCAPE:
						return 27;
					case SDLK_RETURN:
						return 58;
					case '1':
						return 1;
					case '2':
						return 2;
					case '3':
						return 3;
				}
			}
		}
		break;
	}
	return 0;
}

void 	SDL2Gui::drawMap() {

	SDL_RenderClear(this->renderer);

	SDL_Rect DestR;
	DestR.w = (this->squareSize + 1) >> 1;
	DestR.h = (this->squareSize + 1) >> 1;
	for (int i = 0; i < this->count; i++) {
		DestR.x = (i % this->wcount * this->squareSize) >> 1;
		DestR.y = (i / this->wcount * this->squareSize) >> 1;
		SDL_RenderCopy(this->renderer, this->gameTextures["land"], nullptr, &DestR);
	}

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);

	for (size_t i = 1; i < this->wcount; i++) {
		SDL_RenderDrawLine(renderer, (this->squareSize * i) >> 1, 0, (this->squareSize * i) >> 1, this->sizeMap[0]);
	}
	for (size_t i = 1; i < this->hcount; i++) {
		SDL_RenderDrawLine(renderer, 0, (this->squareSize * i) >> 1, this->sizeMap[0], (this->squareSize * i) >> 1);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

}

void 	SDL2Gui::drawBody(int x, int y) {

	SDL_Rect DestR;
	DestR.x = (x + 1) >> 1;
	DestR.y = (y + 1) >> 1;
	DestR.w = (this->squareSize + 1) >> 1;
	DestR.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->gameTextures["body"], nullptr, &DestR);
}

void 	SDL2Gui::drawEnemyBody(int x, int y) {

	SDL_Rect DestR;
	DestR.x = (x + 1) >> 1;
	DestR.y = (y + 1) >> 1;
	DestR.w = (this->squareSize + 1) >> 1;
	DestR.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->gameTextures["enemyBody"], nullptr, &DestR);

}

void 	SDL2Gui::display(int speed) {

	SDL_Delay( speed - (speed / 3) );
	SDL_RenderPresent(this->renderer);

}

void 	SDL2Gui::drawGameOver() {

	SDL_RenderCopy(this->renderer, this->gameOverTexture[this->cadr], NULL, NULL);

	if (++this->cadr == 10) {
		this->cadr >>= 10;
	}

}

void 	SDL2Gui::init() {

	this->menuFont = "./dynLib2/resources/FFF_Tusj.ttf";

	this->token_x = -1;
	this->token_y = -1;
	this->token_n = 0;

	this->enemyToken_x = -1;
	this->enemyToken_y = -1;
	this->enemyToken_n = 0;

	this->cadr = 0;
	this->cadrToken = 0;
	this->cadrWater = 0;

	SDL_Surface *bg = IMG_Load("./dynLib2/resources/images/menu.png");
	if (bg == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *menu = SDL_CreateTextureFromSurface(this->renderer, bg);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("menu", menu));
	SDL_FreeSurface(bg);

	SDL_Surface *head = IMG_Load("./dynLib2/resources/images/head.png");
	if (head == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *headTex = SDL_CreateTextureFromSurface(this->renderer, head);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("head", headTex));
	SDL_FreeSurface(head);

	SDL_Surface *enemyHead = IMG_Load("./dynLib2/resources/images/enemyHead.png");
	if (enemyHead == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *enemyHeadTex = SDL_CreateTextureFromSurface(this->renderer, enemyHead);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("enemyHead", enemyHeadTex));
	SDL_FreeSurface(enemyHead);

	SDL_Surface *header = IMG_Load("./dynLib2/resources/images/header.png");
	if (header == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *headerTex = SDL_CreateTextureFromSurface(this->renderer, header);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("header", headerTex));
	SDL_FreeSurface(header);

	SDL_Surface *hole = IMG_Load("./dynLib2/resources/images/hole.png");
	if (hole == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *holeTex = SDL_CreateTextureFromSurface(this->renderer, hole);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("hole", holeTex));
	SDL_FreeSurface(hole);

	SDL_Surface *pause = IMG_Load("./dynLib2/resources/images/pause.png");
	if (pause == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *pauseTex = SDL_CreateTextureFromSurface(this->renderer, pause);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("pause", pauseTex));
	SDL_FreeSurface(pause);

	SDL_Surface *land = IMG_Load("./dynLib2/resources/images/land.png");
	if (land == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *landTex = SDL_CreateTextureFromSurface(this->renderer, land);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("land", landTex));
	SDL_FreeSurface(land);

	SDL_Surface *body = IMG_Load("./dynLib2/resources/images/body.png");
	if (body == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *bodyTex = SDL_CreateTextureFromSurface(this->renderer, body);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("body", bodyTex));
	SDL_FreeSurface(body);

	SDL_Surface *enemyBody = IMG_Load("./dynLib2/resources/images/enemyBody.png");
	if (enemyBody == nullptr){
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Texture *enemyBodyTex = SDL_CreateTextureFromSurface(this->renderer, enemyBody);
	this->gameTextures.insert(std::pair<std::string, SDL_Texture *>("enemyBody", enemyBodyTex));
	SDL_FreeSurface(enemyBody);

	for (int i = 0; i < 6; i++)
	{
		std::string file = "./dynLib2/resources/loading/" + this->toStr(i % 6) + ".png";
		SDL_Surface *load = IMG_Load(file.c_str());
		if (load == nullptr) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_Texture *loadTex = SDL_CreateTextureFromSurface(this->renderer, load);
		this->loadingTexture[i] = loadTex;
		SDL_FreeSurface(load);
	}

	for (int i = 0; i < 5; i++)
	{
		std::string file = "./dynLib2/resources/tokens/" + this->toStr(i) + ".png";
		SDL_Surface *token = IMG_Load(file.c_str());
		if (token == nullptr) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_Texture *tokenTex = SDL_CreateTextureFromSurface(this->renderer, token);
		this->tokenTexture[i] = tokenTex;
		SDL_FreeSurface(token);
	}

	for (int i = 0; i < 58; i++)
	{
		std::string file = "./dynLib2/resources/water/" + this->toStr(i) + ".png";
		SDL_Surface *water = IMG_Load(file.c_str());
		if (water == nullptr) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_Texture *waterTex = SDL_CreateTextureFromSurface(this->renderer, water);
		this->waterTexture[i] = waterTex;
		SDL_FreeSurface(water);
	}

	for (int i = 0; i < 50; i++)
	{
		std::string file = "./dynLib2/resources/bonusToken/" + this->toStr(i) + ".png";
		SDL_Surface *bonusToken = IMG_Load(file.c_str());
		if (bonusToken == nullptr) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_Texture *bonusTokenTex = SDL_CreateTextureFromSurface(this->renderer, bonusToken);
		this->bonusTexture[i] = bonusTokenTex;
		SDL_FreeSurface(bonusToken);
	}

	for (int i = 0; i < 10; i++)
	{
		std::string file = "./dynLib2/resources/gameOver/" + this->toStr(i) + ".png";
		SDL_Surface *gameOver = IMG_Load(file.c_str());
		if (gameOver == nullptr) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_Texture *gameOverTex = SDL_CreateTextureFromSurface(this->renderer, gameOver);
		this->gameOverTexture[i] = gameOverTex;
		SDL_FreeSurface(gameOver);
	}

	return;
}

void 	SDL2Gui::drawNewGame() {

	std::string file;

	for (int i = 0; i < 20 && this->getch() != 27; i++)
	{
		SDL_RenderClear(this->renderer);
		SDL_RenderCopy(this->renderer, this->loadingTexture[i % 6], NULL, NULL);
		SDL_RenderPresent(this->renderer);
		SDL_Delay(100);
	}

}

void 	SDL2Gui::drawToken(int x, int y) {

	if (this->token_x != x && this->token_y != y) {
		this->token_x = x;
		this->token_y = y;
		this->token_n = std::rand() % 5;
	}

	SDL_Rect tokenRect;
	tokenRect.x = (x + 1) >> 1;
	tokenRect.y = (y + 1) >> 1;
	tokenRect.w = (this->squareSize + 1) >> 1;
	tokenRect.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->tokenTexture[this->token_n], nullptr, &tokenRect);
}

void 	SDL2Gui::drawToken(int x, int y, int) {
	if (this->enemyToken_x != x && this->enemyToken_y != y) {
		this->enemyToken_x = x;
		this->enemyToken_y = y;
		this->enemyToken_n = std::rand() % 5;
	}

	SDL_Rect tokenRect;
	tokenRect.x = (x + 1) >> 1;
	tokenRect.y = (y + 1) >> 1;
	tokenRect.w = (this->squareSize + 1) >> 1;
	tokenRect.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->tokenTexture[this->enemyToken_n], nullptr, &tokenRect);
}

void 	SDL2Gui::drawBonusToken(int x, int y) {

	SDL_Rect tokenRect;
	tokenRect.x = (x + 1) >> 1;
	tokenRect.y = (y + 1) >> 1;
	tokenRect.w = (this->squareSize + 1) >> 1;
	tokenRect.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->bonusTexture[this->cadrToken], nullptr, &tokenRect);

	++this->cadrToken > 49 ? this->cadrToken >>= 10 : 0;

}

void 	SDL2Gui::drawSnakeHead(int x, int y) {

	SDL_Rect DestR;
	DestR.x = (x + 1) >> 1;
	DestR.y = (y + 1) >> 1;
	DestR.w = (this->squareSize + 1) >> 1;
	DestR.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->gameTextures["head"], nullptr, &DestR);
}

void 	SDL2Gui::drawEnemyHead(int x, int y) {

	SDL_Rect DestR;
	DestR.x = (x + 1) >> 1;
	DestR.y = (y + 1) >> 1;
	DestR.w = (this->squareSize + 1) >> 1;
	DestR.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->gameTextures["enemyHead"], nullptr, &DestR);
}

void 	SDL2Gui::drawPause() {
	SDL_Rect DestR;
	DestR.w = this->squareSize * 10;
	DestR.h = DestR.w / 5;
	DestR.x = (this->sizeMap[0] >> 2) - (DestR.w >> 1);
	DestR.y = (this->sizeMap[1] >> 2) - (DestR.h >> 1);

	SDL_RenderCopy(this->renderer, this->gameTextures["pause"], nullptr, &DestR);
	SDL_RenderPresent(this->renderer);

}

void 	SDL2Gui::drawScores(int scores, int highScore) {

	SDL_Rect DestR;
	DestR.x = 0;
	DestR.y = 0;
	DestR.w = (this->sizeMap[0] + 1) >> 1;
	DestR.h = this->squareSize;

	SDL_RenderCopy(this->renderer, this->gameTextures["header"], nullptr, &DestR);

	TTF_Font *font = TTF_OpenFont((this->menuFont).c_str(), this->sizeMap[1] >> 5);
	SDL_Color color = { 255, 255, 255 };
	SDL_Color scoreColor = { 255, 255, 0 };
	SDL_Color highScoreColor = { 255, 0, 0 };

	std::string scoreText_main = "Score: ";

	SDL_Surface *scoreSurface_main = TTF_RenderText_Solid(font, scoreText_main.c_str(), color);
	SDL_Texture *scoreTex_main = SDL_CreateTextureFromSurface(this->renderer, scoreSurface_main);
	SDL_Rect scoreRect_main;
	scoreRect_main.h = (this->sizeMap[1] / 43);
	scoreRect_main.w = 7 * (this->sizeMap[0] >> 6);
	scoreRect_main.x = this->squareSize >> 1;
	scoreRect_main.y = 0;

	std::string scoreText = this->toStr(scores);

	SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), scoreColor);
	SDL_Texture *scoreTex = SDL_CreateTextureFromSurface(this->renderer, scoreSurface);
	SDL_Rect scoreRect;
	scoreRect.h = (this->sizeMap[1] / 43);
	scoreRect.w = scoreText.length() * (this->sizeMap[0] >> 6);
	scoreRect.x = scoreRect_main.x + scoreRect_main.w;
	scoreRect.y = 0;

	std::string highScoreText_main = "Highscore: ";

	SDL_Surface *highScoreSurface_main = TTF_RenderText_Solid(font, highScoreText_main.c_str(), color);
	SDL_Texture *highScoreTex_main = SDL_CreateTextureFromSurface(this->renderer, highScoreSurface_main);
	SDL_Rect highScoreRect_main;
	highScoreRect_main.h = (this->sizeMap[1] / 43);
	highScoreRect_main.w = 11 * (this->sizeMap[0] >> 6);
	highScoreRect_main.x = this->sizeMap[0] >> 2;
	highScoreRect_main.y = 0;

	std::string highScoreText = this->toStr(highScore);

	SDL_Surface *highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), highScoreColor);
	SDL_Texture *highScoreTex = SDL_CreateTextureFromSurface(this->renderer, highScoreSurface);
	SDL_Rect highScoreRect;
	highScoreRect.h = (this->sizeMap[1] / 43);
	highScoreRect.w = highScoreText.length() * (this->sizeMap[0] >> 6);
	highScoreRect.x = highScoreRect_main.x + highScoreRect_main.w;
	highScoreRect.y = 0;

	SDL_RenderCopy(this->renderer, scoreTex_main, NULL, &scoreRect_main);
	SDL_RenderCopy(this->renderer, scoreTex, NULL, &scoreRect);
	SDL_RenderCopy(this->renderer, highScoreTex_main, NULL, &highScoreRect_main);
	SDL_RenderCopy(this->renderer, highScoreTex, NULL, &highScoreRect);

	SDL_FreeSurface(scoreSurface_main);
	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(highScoreSurface_main);
	SDL_FreeSurface(highScoreSurface);
	SDL_DestroyTexture(scoreTex_main);
	SDL_DestroyTexture(scoreTex);
	SDL_DestroyTexture(highScoreTex_main);
	SDL_DestroyTexture(highScoreTex);

	TTF_CloseFont(font);
	font = nullptr;

}

void 	SDL2Gui::drawBarrier(int x, int y) {

	SDL_Rect holeRect;
	holeRect.x = (x + 1) >> 1;
	holeRect.y = (y + 1) >> 1;
	holeRect.w = (this->squareSize + 1) >> 1;
	holeRect.h = (this->squareSize + 1) >> 1;

	SDL_RenderCopy(this->renderer, this->gameTextures["hole"], nullptr, &holeRect);
}

std::string 	SDL2Gui::toStr(int num) {
	std::stringstream 	ss;
	ss << num;

	return ss.str();
}

void 	SDL2Gui::drawSplitter() {

	SDL_Rect waterRect;
	waterRect.x = (this->squareSize * ((this->wcount + 1) >> 1)) >> 1;
	waterRect.w = ((this->squareSize + 1) >> 1) + 1;
	waterRect.h = ((this->squareSize + 1) >> 1) + 1;
	waterRect.y = 0;

	for (int i = 1; i < this->hcount + 1; i++) {
		SDL_RenderCopy(this->renderer, this->waterTexture[this->cadrWater], nullptr, &waterRect);
		waterRect.y += ((this->squareSize + 1) >> 1);
	}

	++this->cadrWater > 57 ? this->cadrWater >>= 10 : 0;
}

void 	SDL2Gui::drawScoresMultiplayer(int scores, int enemyScores) {

	SDL_Rect DestR;
	DestR.x = 0;
	DestR.y = 0;
	DestR.w = (this->sizeMap[0] + 1) >> 1;
	DestR.h = this->squareSize;

	SDL_RenderCopy(this->renderer, this->gameTextures["header"], nullptr, &DestR);

	TTF_Font *font = TTF_OpenFont((this->menuFont).c_str(), this->sizeMap[1] >> 5);
	SDL_Color color = { 255, 255, 255 };
	SDL_Color scoreColor = { 0, 255, 0 };
	SDL_Color enemyColor = { 255, 0, 0 };

	std::string scoreText_main = "You: ";

	SDL_Surface *scoreSurface_main = TTF_RenderText_Solid(font, scoreText_main.c_str(), color);
	SDL_Texture *scoreTex_main = SDL_CreateTextureFromSurface(this->renderer, scoreSurface_main);
	SDL_Rect scoreRect_main;
	scoreRect_main.h = (this->sizeMap[1] / 43);
	scoreRect_main.w = 5 * (this->sizeMap[0] >> 6);
	scoreRect_main.x = this->squareSize;
	scoreRect_main.y = 0;

	std::string scoreText = this->toStr(scores);

	SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), scoreColor);
	SDL_Texture *scoreTex = SDL_CreateTextureFromSurface(this->renderer, scoreSurface);
	SDL_Rect scoreRect;
	scoreRect.h = (this->sizeMap[1] / 43);
	scoreRect.w = scoreText.length() * (this->sizeMap[0] >> 6);
	scoreRect.x = scoreRect_main.x + scoreRect_main.w;
	scoreRect.y = 0;

	std::string enemyScoreText_main = "Bot: ";

	SDL_Surface *enemyScoreSurface_main = TTF_RenderText_Solid(font, enemyScoreText_main.c_str(), color);
	SDL_Texture *enemyScoreTex_main = SDL_CreateTextureFromSurface(this->renderer, enemyScoreSurface_main);
	SDL_Rect enemyScoreRect_main;
	enemyScoreRect_main.h = (this->sizeMap[1] / 43);
	enemyScoreRect_main.w = 5 * (this->sizeMap[0] >> 6);
	enemyScoreRect_main.x = (this->sizeMap[0] >> 2) + (this->sizeMap[0] >> 3) - this->squareSize;
	enemyScoreRect_main.y = 0;

	std::string enemyScoreText = this->toStr(enemyScores);

	SDL_Surface *enemyScoreSurface = TTF_RenderText_Solid(font, enemyScoreText.c_str(), enemyColor);
	SDL_Texture *enemyScoreTex = SDL_CreateTextureFromSurface(this->renderer, enemyScoreSurface);
	SDL_Rect enemyScoreRect;
	enemyScoreRect.h = (this->sizeMap[1] / 43);
	enemyScoreRect.w = enemyScoreText.length() * (this->sizeMap[0] >> 6);
	enemyScoreRect.x = enemyScoreRect_main.x + enemyScoreRect_main.w;
	enemyScoreRect.y = 0;

	SDL_RenderCopy(this->renderer, scoreTex_main, NULL, &scoreRect_main);
	SDL_RenderCopy(this->renderer, scoreTex, NULL, &scoreRect);
	SDL_RenderCopy(this->renderer, enemyScoreTex_main, NULL, &enemyScoreRect_main);
	SDL_RenderCopy(this->renderer, enemyScoreTex, NULL, &enemyScoreRect);

	SDL_FreeSurface(scoreSurface_main);
	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(enemyScoreSurface_main);
	SDL_FreeSurface(enemyScoreSurface);
	SDL_DestroyTexture(scoreTex_main);
	SDL_DestroyTexture(scoreTex);
	SDL_DestroyTexture(enemyScoreTex_main);
	SDL_DestroyTexture(enemyScoreTex);

	TTF_CloseFont(font);
	font = nullptr;
}

void 	SDL2Gui::drawTimer(int timer) {

	time_t		t = static_cast<time_t>(60 - timer);
	struct tm	*time_stamp = gmtime(&t);
	char		buff[6];

	time_stamp = gmtime(&t);
	strftime(buff, 6, "%M:%S\0", time_stamp);

	TTF_Font *font = TTF_OpenFont((this->menuFont).c_str(), this->sizeMap[0] >> 5);
	SDL_Color color = { 255, 255, 255 };
	SDL_Color noTimeColor = { 255, 0, 0 };

	SDL_Surface *timerSurface = TTF_RenderText_Solid(font, buff, 60 - timer <= 5 ? noTimeColor : color);
	SDL_Texture *timerTex = SDL_CreateTextureFromSurface(this->renderer, timerSurface);
	SDL_Rect timerRect;
	timerRect.h = (this->sizeMap[1] / 43);
	timerRect.w = 5 * (this->sizeMap[0] >> 6);
	timerRect.x = (this->sizeMap[0] >> 2) - (timerRect.w >> 1) + (this->squareSize >> 2);
	timerRect.y = 0;

	SDL_RenderCopy(this->renderer, timerTex, NULL, &timerRect);

	SDL_FreeSurface(timerSurface);
	SDL_DestroyTexture(timerTex);

	TTF_CloseFont(font);
	font = nullptr;
}

void 	SDL2Gui::drawWinner(std::string const &winner) {

	TTF_Font *font = TTF_OpenFont((this->menuFont).c_str(), this->sizeMap[0] >> 4);
	SDL_Color color = { 255, 255, 255 };
	SDL_Color youColor = { 0, 255, 0 };
	SDL_Color enemyColor = { 255, 0, 0 };

	std::string winnerText_main = "Winner: ";

	float y_0 = (this->squareSize >> 1) * 0.89f * this->hcount;
	float y_1 = (this->squareSize >> 1) * 0.94f * this->hcount;

	SDL_Surface *winnerSurface_main = TTF_RenderText_Solid(font, winnerText_main.c_str(), color);
	SDL_Texture *winnerTex_main = SDL_CreateTextureFromSurface(this->renderer, winnerSurface_main);
	SDL_Rect winnerRect_main;
	winnerRect_main.h = (this->sizeMap[1] / 43);
	winnerRect_main.w = 8 * (this->sizeMap[0] >> 6);
	winnerRect_main.x = (this->sizeMap[0] >> 2) - (winnerRect_main.w >> 1);
	winnerRect_main.y = y_0;

	SDL_Surface *winnerSurface = TTF_RenderText_Solid(font, winner.c_str(), (winner == "Bot" ? enemyColor : youColor));
	SDL_Texture *winnerTex = SDL_CreateTextureFromSurface(this->renderer, winnerSurface);
	SDL_Rect winnerRect;
	winnerRect.h = (this->sizeMap[1] / 43);
	winnerRect.w = winner.length() * (this->sizeMap[0] >> 6);
	winnerRect.x = (this->sizeMap[0] >> 2) - (winnerRect.w >> 1);
	winnerRect.y = y_1;

	SDL_RenderCopy(this->renderer, winnerTex_main, NULL, &winnerRect_main);
	SDL_RenderCopy(this->renderer, winnerTex, NULL, &winnerRect);

	SDL_FreeSurface(winnerSurface_main);
	SDL_FreeSurface(winnerSurface);
	SDL_DestroyTexture(winnerTex_main);
	SDL_DestroyTexture(winnerTex);

	TTF_CloseFont(font);
	font = nullptr;
}

void 	SDL2Gui::drawMenu(int cursor) {

	// Creating text objects

	TTF_Font *font = TTF_OpenFont((this->menuFont).c_str(), this->sizeMap[0] >> 4);
	SDL_Color color = { 255, 255, 255 };
	SDL_Color actColor = { 255, 0, 0 };

	SDL_Surface *newGame = TTF_RenderText_Solid(font, "Start game", cursor == 0 ? actColor : color);
	SDL_Surface *bot = TTF_RenderText_Solid(font, "Play vs bot", cursor == 1 ? actColor : color);
	SDL_Surface *quit = TTF_RenderText_Solid(font, "Leave game", cursor == 2 ? actColor : color);

	SDL_Texture *newGameTex = SDL_CreateTextureFromSurface(this->renderer, newGame);
	SDL_Texture *botTex = SDL_CreateTextureFromSurface(this->renderer, bot);
	SDL_Texture *quitTex = SDL_CreateTextureFromSurface(this->renderer, quit);

	// Rendering

	SDL_Rect newGameRect;
	newGameRect.x = this->sizeMap[0] >> 4;
	newGameRect.y = this->sizeMap[1] >> 2;
	newGameRect.w = this->sizeMap[0] >> 3;
	newGameRect.h = this->sizeMap[1] >> 5;

	SDL_Rect botRect;
	botRect.x = this->sizeMap[0] >> 4;
	botRect.y = newGameRect.y + (newGameRect.h) + (this->sizeMap[1] >> 8);
	botRect.w = this->sizeMap[0] >> 3;
	botRect.h = this->sizeMap[1] >> 5;

	SDL_Rect quitRect;
	quitRect.x = this->sizeMap[0] >> 4;
	quitRect.y = botRect.y + (botRect.h) + (this->sizeMap[1] >> 8);
	quitRect.w = this->sizeMap[0] >> 3;
	quitRect.h = this->sizeMap[1] >> 5;

	SDL_Rect headRect;
	headRect.w = newGameRect.h >> 1;
	headRect.h = newGameRect.h >> 1;
	headRect.x = newGameRect.x - (this->sizeMap[0] / 40);
	headRect.y = newGameRect.y + ((newGameRect.h - headRect.h) >> 1) + (botRect.y - newGameRect.y) * cursor;

	SDL_RenderCopy(this->renderer, this->gameTextures["menu"], NULL, NULL);
	SDL_RenderCopy(this->renderer, newGameTex, NULL, &newGameRect);
	SDL_RenderCopy(this->renderer, botTex, NULL, &botRect);
	SDL_RenderCopy(this->renderer, quitTex, NULL, &quitRect);
	SDL_RenderCopy(this->renderer, this->gameTextures["head"], NULL, &headRect);

	// Cleaning

	SDL_FreeSurface(newGame);
	SDL_FreeSurface(bot);
	SDL_FreeSurface(quit);
	SDL_DestroyTexture(newGameTex);
	SDL_DestroyTexture(botTex);
	SDL_DestroyTexture(quitTex);

	TTF_CloseFont(font);
	font = nullptr;

}

void	SDL2Gui::playSound(int num) {
	switch (num)
	{
		case 0:
			system("afplay ./dynLib2/resources/sounds/menuMove.wav &");
			break;
		case 1:
			system("afplay ./dynLib2/resources/sounds/menuSelect.wav &");
			break;
		case 2:
			system("afplay ./dynLib2/resources/sounds/token.wav &");
			break;
		case 3:
			system("afplay ./dynLib2/resources/sounds/enemyToken.wav &");
			break;
		default:
			system("afplay ./dynLib2/resources/sounds/gameOver.wav &");
	}
}

/* C Functions */

extern "C" IGui* getGui(int squareSize) {
	return new SDL2Gui(squareSize);
}
