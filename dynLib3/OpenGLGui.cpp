#include "OpenGLGui.hpp"

int pressed;

void window_close_callback(GLFWwindow* window) {

	pressed = 27;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				pressed = 27;
				break;
			case GLFW_KEY_RIGHT:
				pressed = 72;
				break;
			case GLFW_KEY_LEFT:
				pressed = 71;
				break;
			case GLFW_KEY_UP:
				pressed = 73;
				break;
			case GLFW_KEY_DOWN:
				pressed = 74;
				break;
			case GLFW_KEY_SPACE:
				pressed = 57;
				break;
			case GLFW_KEY_ENTER:
				pressed = 58;
				break;
			case GLFW_KEY_P:
				pressed = 15;
				break;
			case GLFW_KEY_1:
				pressed = 1;
				break;
			case GLFW_KEY_2:
				pressed = 2;
				break;
			case GLFW_KEY_3:
				pressed = 3;
				break;
		}
	}

}

OpenGLGui::OpenGLGui(int squareSize) : squareSize(squareSize), start(0) {

	if (!glfwInit()) {
		std::cout << "ERROR: GLFW was not initialized" << std::endl;
		exit(EXIT_FAILURE);
	}
}

OpenGLGui::OpenGLGui(OpenGLGui const &obj) {
	*this = obj;
}

OpenGLGui::~OpenGLGui() { }

OpenGLGui	&OpenGLGui::operator = (OpenGLGui const &) {
	return *this;
}

void	OpenGLGui::openWindow(int width, int height) {

	this->sizeMap[0] = width;
	this->sizeMap[1] = height;
	this->wcount = width / this->squareSize;
	this->hcount = height / this->squareSize;
	this->count = this->hcount * this->wcount;
	this->font = new FTGLPixmapFont("./dynLib3/resources/Luminari.ttf");
	this->font->FaceSize(static_cast<int>(this->squareSize << 1));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	this->window = glfwCreateWindow(this->sizeMap[0] >> 1, this->sizeMap[1] >> 1, "Nibbler OpenGL / GLFW", NULL, NULL);
	glfwSetWindowPos(this->window, (5120 - width) >> 2, (2880 - height) >> 2);
	glfwMakeContextCurrent(this->window);
	this->init();
	glfwSetKeyCallback(this->window, key_callback);
	glfwSetWindowCloseCallback(this->window, window_close_callback);
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelTransferf(GL_RED_BIAS, -0.5f);
	glPixelTransferf(GL_GREEN_BIAS, -1.0f);
	glPixelTransferf(GL_BLUE_BIAS, -1.0f);
	this->font->FaceSize(this->sizeMap[0] >> 4);
	this->font->Render("Loading   resources...", 22, FTPoint(((this->sizeMap[0] >> 2) - this->squareSize), (this->sizeMap[1] >> 1)));
	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);
	glfwSwapBuffers(this->window);

}

void	OpenGLGui::closeWindow() {

	delete this->font;
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

int		OpenGLGui::getch() {

	glfwPollEvents();
	int temp = pressed;
	pressed = 0;

	return temp;
}

void 	OpenGLGui::drawMap() {

	glfwMakeContextCurrent(this->window);

	for (int i = 0; i < this->count; i++) {
		glViewport(this->squareSize * (i % this->wcount), this->squareSize * (this->hcount - 2 - i / this->wcount), this->squareSize, this->squareSize);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->gameTextures["land"]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(-1.f, -1.f);
		glTexCoord2f(1.f, 0.f);
		glVertex2f(1.f, -1.f);
		glTexCoord2f(1.f, 1.f);
		glVertex2f(1.f, 1.f);
		glTexCoord2f(0.f, 1.f);
		glVertex2f(-1.f, 1.f);
		glEnd();
	}

	glViewport(0, 0, this->sizeMap[0], this->sizeMap[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	for (float i = 1; i < this->wcount; i++) {
		glBegin (GL_LINES);
			glVertex2f (-1 + (i * 2 / this->wcount), 1);
			glVertex2f (-1 + (i * 2 / this->wcount), -1);
		glEnd();
	}
	for (float i = this->hcount; i > 0; i--) {
		glBegin (GL_LINES);
			glVertex2f (-1, -1 + (i * 2 / this->hcount));
			glVertex2f (1, -1 + (i * 2 / this->hcount));
		glEnd();
	}
}

void 	OpenGLGui::drawBody(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["body"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

}

void 	OpenGLGui::drawEnemyBody(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["enemyBody"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();
}

void 	OpenGLGui::display(int speed) {

	glfwSwapBuffers(this->window);
	std::this_thread::sleep_for(std::chrono::milliseconds(speed));
}

void 	OpenGLGui::drawGameOver() {

	glfwMakeContextCurrent(this->window);

	glViewport(0, 0, this->sizeMap[0], this->sizeMap[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["main"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

	float transpIndex = 1.0f / (this->cadr++ % 10 + 1);

	glPixelTransferf(GL_RED_BIAS, -0.09f);
	glPixelTransferf(GL_GREEN_BIAS, -0.25f);
	glPixelTransferf(GL_BLUE_BIAS, -0.54f);
	glPixelTransferf(GL_ALPHA_BIAS, -1.0f + transpIndex);
	this->font->FaceSize(static_cast<int>(this->squareSize * 3));
	this->font->Render("GAME OVER", 9, FTPoint(((this->sizeMap[0] >> 2)), (this->sizeMap[1] >> 1) - this->squareSize));
	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);
	glPixelTransferf(GL_ALPHA_BIAS, 0.0f);

}

void 	OpenGLGui::init() {

	this->token_x = -1;
	this->token_y = -1;
	this->token_n = 0;

	this->enemyToken_x = -1;
	this->enemyToken_y = -1;
	this->enemyToken_n = 0;

	this->cadr = 0;
	this->cadrToken = 0;
	this->cadrWater = 0;

	glfwMakeContextCurrent(this->window);

	this->gameTextures.insert ( std::pair<std::string, GLuint>("main", SOIL_load_OGL_texture(
		"./dynLib3/resources/images/main.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	)) );

}

void 	OpenGLGui::init_rest() {

	glfwMakeContextCurrent(this->window);

	this->start = 1;

	for (int i = 0; i < 6; i++) {
		std::string file = "./dynLib3/resources/loading/" + this->toStr(i) + ".jpg";
		this->loadingTexture[i] = SOIL_load_OGL_texture(
			file.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	}

	for (int i = 0; i < 5; i++) {
		std::string file = "./dynLib3/resources/gems/gem" + this->toStr(i) + ".png";
		this->gemTexture[i] = SOIL_load_OGL_texture(
			file.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	}

	for (int i = 0; i < 6; i++) {
		std::string file = "./dynLib3/resources/bonus/" + this->toStr(i) + ".jpg";
		this->bonusTexture[i] = SOIL_load_OGL_texture(
			file.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	}

	std::string file;
	for (int i = 0; i < 5; i++) {
		std::string file = "./dynLib3/resources/end/" + this->toStr(i) + ".jpg";
		this->endTexture[i] = SOIL_load_OGL_texture(
			file.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	}

	std::string name = "";

	for (int i = 0; i < 9; i++) {
		switch (i) {
			case 0:
				name = "land";
				break;
			case 1:
				name = "head";
				break;
			case 2:
				name = "body";
				break;
			case 3:
				name = "trap";
				break;
			case 4:
				name = "pause";
				break;
			case 5:
				name = "header";
				break;
			case 6:
				name = "enemyHead";
				break;
			case 7:
				name = "enemyBody";
				break;
			case 8:
				name = "splitter";
				break;
		}

		std::string file = "./dynLib3/resources/images/" + name + ".png";
		this->gameTextures.insert ( std::pair<std::string, GLuint>(name, SOIL_load_OGL_texture(
			file.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		)) );
	}
}

void 	OpenGLGui::drawNewGame() {

	glfwMakeContextCurrent(this->window);
	glViewport(0, 0, this->sizeMap[0], this->sizeMap[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	for (int i = 0; i < 150; i++) {

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->loadingTexture[i % 6]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(-1.f, -1.f);
		glTexCoord2f(1.f, 0.f);
		glVertex2f(1.f, -1.f);
		glTexCoord2f(1.f, 1.f);
		glVertex2f(1.f, 1.f);
		glTexCoord2f(0.f, 1.f);
		glVertex2f(-1.f, 1.f);
		glEnd();

		glfwSwapBuffers(this->window);
		glfwWaitEventsTimeout(0.01);
		if (pressed == 27) {
			break;
		}
	}
	pressed = 0;
}

void 	OpenGLGui::drawToken(int x, int y) {

	if (this->token_x != x && this->token_y != y) {
		this->token_x = x;
		this->token_y = y;
		this->token_n = std::rand() % 5;
	}

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gemTexture[this->token_n]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

}

void 	OpenGLGui::drawToken(int x, int y, int) {

	if (this->enemyToken_x != x && this->enemyToken_y != y) {
		this->enemyToken_x = x;
		this->enemyToken_y = y;
		this->enemyToken_n = std::rand() % 5;
	}

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gemTexture[this->enemyToken_n]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();


}

void 	OpenGLGui::drawBonusToken(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->bonusTexture[this->cadrToken++ % 6]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

}

void 	OpenGLGui::drawSnakeHead(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["head"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();
}

void 	OpenGLGui::drawEnemyHead(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["enemyHead"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

}

void 	OpenGLGui::drawPause() {

	glfwMakeContextCurrent(this->window);

	glViewport(0, (this->sizeMap[1] >> 1) - this->squareSize, this->sizeMap[0], this->squareSize << 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["pause"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

	glfwSwapBuffers(this->window);

}

void 	OpenGLGui::drawScores(int scores, int highScore) {

	glViewport(0, this->sizeMap[1] - (this->squareSize << 1), this->sizeMap[0], this->squareSize << 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["header"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

	this->font->FaceSize((this->squareSize >> 1) + this->squareSize);
	std::string score = "Score: " + this->toStr(scores);
	std::string hscore = "Highscore: " + this->toStr(highScore);
	this->font->Render(score.c_str(), score.length(), FTPoint(this->squareSize, this->sizeMap[1] - (this->squareSize) - (this->squareSize >> 1)));
	this->font->Render(hscore.c_str(), hscore.length(), FTPoint(this->squareSize + (this->sizeMap[0] >> 1), this->sizeMap[1] - (this->squareSize) - (this->squareSize >> 1)));
}

void 	OpenGLGui::drawBarrier(int x, int y) {

	glfwMakeContextCurrent(this->window);

	glViewport(x + 1, this->sizeMap[1] - y - this->squareSize + 1, this->squareSize - 2, this->squareSize - 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["trap"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

}

std::string 	OpenGLGui::toStr(int num) {
	std::stringstream 	ss;
	ss << num;

	return ss.str();
}

void 	OpenGLGui::drawSplitter() {

	glfwMakeContextCurrent(this->window);

	for (int i = 1; i < this->hcount; i++) {
		glViewport((this->squareSize * ((this->wcount + 1) >> 1)), this->squareSize * (this->hcount - 2 - i) + 1, this->squareSize - 2, this->squareSize - 2);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->gameTextures["splitter"]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(-1.f, -1.f);
		glTexCoord2f(1.f, 0.f);
		glVertex2f(1.f, -1.f);
		glTexCoord2f(1.f, 1.f);
		glVertex2f(1.f, 1.f);
		glTexCoord2f(0.f, 1.f);
		glVertex2f(-1.f, 1.f);
		glEnd();
	}
}

void 	OpenGLGui::drawScoresMultiplayer(int scores, int enemyScores) {

	glViewport(0, this->sizeMap[1] - (this->squareSize << 1), this->sizeMap[0], this->squareSize << 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["header"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();
	this->font->FaceSize((this->squareSize >> 1) + this->squareSize);
	std::string score = "You: " + this->toStr(scores);
	std::string escore = "Bot: " + this->toStr(enemyScores);
	this->font->Render(score.c_str(), score.length(), FTPoint(this->squareSize << 2, this->sizeMap[1] - (this->squareSize) - (this->squareSize >> 1)));
	this->font->Render(escore.c_str(), escore.length(), FTPoint((this->sizeMap[0] >> 1) + (this->sizeMap[0] >> 2) - this->squareSize, this->sizeMap[1] - (this->squareSize) - (this->squareSize >> 1)));

}

void 	OpenGLGui::drawTimer(int timer) {

	time_t		t = static_cast<time_t>(60 - timer);
	struct tm	*time_stamp = gmtime(&t);
	char		buff[6];

	time_stamp = gmtime(&t);
	strftime(buff, 6, "%M:%S\0", time_stamp);

	this->font->FaceSize((this->squareSize >> 1) + this->squareSize);
	this->font->Render(buff, 5, FTPoint((this->sizeMap[0] >> 1) - (this->squareSize), this->sizeMap[1] - (this->squareSize) - (this->squareSize >> 1)));

}

void 	OpenGLGui::drawWinner(std::string const &winner) {

	std::string winnerFull = "";
	glPixelTransferf(GL_ALPHA_BIAS, 0.0f);

	if (winner == "Bot") {
		winnerFull = "You lose :(";
		glPixelTransferf(GL_RED_BIAS, -0.4f);
		glPixelTransferf(GL_GREEN_BIAS, -1.0f);
		glPixelTransferf(GL_BLUE_BIAS, -1.0f);
	} else if (winner == "You") {
		winnerFull = "You won! :)";
		glPixelTransferf(GL_RED_BIAS, -1.0f);
		glPixelTransferf(GL_GREEN_BIAS, -0.4f);
		glPixelTransferf(GL_BLUE_BIAS, -1.0f);
	}

	this->font->FaceSize((this->squareSize >> 1) + this->squareSize);
	this->font->Render(winnerFull.c_str(), 11, FTPoint((this->sizeMap[0] >> 1) - (this->squareSize << 2), (this->sizeMap[1] >> 2)));

	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);
}

void 	OpenGLGui::drawMenu(int cursor) {

	glfwMakeContextCurrent(this->window);
	glPixelTransferf(GL_ALPHA_BIAS, 0.0f);

	if (this->start == 0) {

		glViewport(0, 0, this->sizeMap[0], this->sizeMap[1]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->gameTextures["main"]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(-1.f, -1.f);
		glTexCoord2f(1.f, 0.f);
		glVertex2f(1.f, -1.f);
		glTexCoord2f(1.f, 1.f);
		glVertex2f(1.f, 1.f);
		glTexCoord2f(0.f, 1.f);
		glVertex2f(-1.f, 1.f);
		glEnd();

		this->font->FaceSize(static_cast<float>(this->squareSize * 2.7));

		if (cursor == 0) {
			glPixelTransferf(GL_RED_BIAS, -0.09f);
			glPixelTransferf(GL_GREEN_BIAS, -0.25f);
			glPixelTransferf(GL_BLUE_BIAS, -0.54f);
		} else {
			glPixelTransferf(GL_RED_BIAS, -0.3f);
			glPixelTransferf(GL_GREEN_BIAS, -0.3f);
			glPixelTransferf(GL_BLUE_BIAS, -0.3f);
		}
		this->font->Render("New game", 8, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 6.4)), (this->sizeMap[1] >> 1) + (this->squareSize << 2)));
		if (cursor == 1) {
			glPixelTransferf(GL_RED_BIAS, -0.09f);
			glPixelTransferf(GL_GREEN_BIAS, -0.25f);
			glPixelTransferf(GL_BLUE_BIAS, -0.54f);
		} else {
			glPixelTransferf(GL_RED_BIAS, -0.3f);
			glPixelTransferf(GL_GREEN_BIAS, -0.3f);
			glPixelTransferf(GL_BLUE_BIAS, -0.3f);
		}
		this->font->Render("Play vs bot", 11, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 7)), (this->sizeMap[1] >> 1)));
		if (cursor == 2) {
			glPixelTransferf(GL_RED_BIAS, -0.09f);
			glPixelTransferf(GL_GREEN_BIAS, -0.25f);
			glPixelTransferf(GL_BLUE_BIAS, -0.54f);
		} else {
			glPixelTransferf(GL_RED_BIAS, -0.3f);
			glPixelTransferf(GL_GREEN_BIAS, -0.3f);
			glPixelTransferf(GL_BLUE_BIAS, -0.3f);
		}
		this->font->Render("Exit", 4, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 2.7)), (this->sizeMap[1] >> 1) - (this->squareSize << 2)));

		glPixelTransferf(GL_RED_BIAS, 0.0f);
		glPixelTransferf(GL_GREEN_BIAS, 0.0f);
		glPixelTransferf(GL_BLUE_BIAS, 0.0f);

		this->init_rest();

		glfwSwapBuffers(this->window);
	}

	glViewport(0, 0, this->sizeMap[0], this->sizeMap[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->gameTextures["main"]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(-1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex2f(1.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, 1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex2f(-1.f, 1.f);
	glEnd();

	this->font->FaceSize(static_cast<float>(this->squareSize * 2.7));

	if (cursor == 0) {
		glPixelTransferf(GL_RED_BIAS, -0.09f);
		glPixelTransferf(GL_GREEN_BIAS, -0.25f);
		glPixelTransferf(GL_BLUE_BIAS, -0.54f);
	} else {
		glPixelTransferf(GL_RED_BIAS, -0.3f);
		glPixelTransferf(GL_GREEN_BIAS, -0.3f);
		glPixelTransferf(GL_BLUE_BIAS, -0.3f);
	}
	this->font->Render("New game", 8, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 6.4)), (this->sizeMap[1] >> 1) + (this->squareSize << 2)));
	if (cursor == 1) {
		glPixelTransferf(GL_RED_BIAS, -0.09f);
		glPixelTransferf(GL_GREEN_BIAS, -0.25f);
		glPixelTransferf(GL_BLUE_BIAS, -0.54f);
	} else {
		glPixelTransferf(GL_RED_BIAS, -0.3f);
		glPixelTransferf(GL_GREEN_BIAS, -0.3f);
		glPixelTransferf(GL_BLUE_BIAS, -0.3f);
	}
	this->font->Render("Play vs bot", 11, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 7)), (this->sizeMap[1] >> 1)));
	if (cursor == 2) {
		glPixelTransferf(GL_RED_BIAS, -0.09f);
		glPixelTransferf(GL_GREEN_BIAS, -0.25f);
		glPixelTransferf(GL_BLUE_BIAS, -0.54f);
	} else {
		glPixelTransferf(GL_RED_BIAS, -0.3f);
		glPixelTransferf(GL_GREEN_BIAS, -0.3f);
		glPixelTransferf(GL_BLUE_BIAS, -0.3f);
	}
	this->font->Render("Exit", 4, FTPoint(((this->sizeMap[0] >> 1) - (this->squareSize * 2.7)), (this->sizeMap[1] >> 1) - (this->squareSize << 2)));

	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);

	glfwWaitEvents();

}

void	OpenGLGui::playSound(int num) {
	switch (num)
	{
		case 0:
			system("afplay ./dynLib3/resources/sounds/menuMove.wav &");
			break;
		case 1:
			system("afplay ./dynLib3/resources/sounds/menuSelect.wav &");
			break;
		case 2:
			system("afplay ./dynLib3/resources/sounds/token.wav &");
			break;
		case 3:
			system("afplay ./dynLib3/resources/sounds/enemyToken.wav &");
			break;
		default:
			system("afplay ./dynLib3/resources/sounds/gameOver.wav &");
	}
}

/* C Functions */

extern "C" IGui* getGui(int squareSize) {
	return new OpenGLGui(squareSize);
}
