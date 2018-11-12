# include "../inc/Controller.hpp"

Controller::Controller(char **parameters, int countLibs) : cursor(0), snake(nullptr), countLibs(countLibs), numOfLib(1), dl_handle(NULL), gui(nullptr)
{
	this->parseParameters(parameters);
	this->calcMap();
	this->loadLib(1);
}

Controller::Controller(Controller const &obj)
{
	*this = obj;
}

Controller::~Controller()
{
	delete this->gui;
	dlclose(this->dl_handle);
}

Controller 	&Controller::operator = (Controller const &)
{
	return *this;
}

/***********************************/

void	Controller::menuGame()
{
	int		button = 0;
	int 	newLib = 0;

	gui->openWindow(this->mapSize.x, this->mapSize.y);

	while ( (button = this->handleCursor()) != -2 )
	{
		if (button > 0 && button < 4)
			this->reloadGui(button);
		else if (button == -3)
			this->snake = new Snake(this->mapSize, this->blockSize);
		else if (button == -1)
			this->snake = new Multiplayer(this->mapSize, this->blockSize);

		if (snake != nullptr)
		{
			snake->setGui(this->gui);
			snake->setLibsInfo(this->countLibs, this->numOfLib);
			newLib = snake->startGame();

			if (newLib != 0)
			{
				this->reloadGui(newLib);
				newLib = 0;
			}
			else
			{
				delete snake;
				snake = nullptr;
			}
		}

		gui->drawMenu(cursor);
		gui->display(0);
	}
	gui->closeWindow();
}

/***********************************/

void	Controller::calcMap()
{
	this->blockSize = ( this->mapSize.x + this->mapSize.y ) / 80;

	this->mapSize.x -= this->mapSize.x % this->blockSize;
	this->mapSize.y -= this->mapSize.y % this->blockSize;
}

void	Controller::parseParameters(char **parameters)
{
	if ( std::regex_match(parameters[0], std::regex("[0-9]+")) == false
	|| 	 std::regex_match(parameters[1], std::regex("[0-9]+")) == false )
	{
		throw std::logic_error("Only digits in map-size parameters");
	}

	this->mapSize.x = atoi( parameters[0] );
	this->mapSize.y = atoi( parameters[1] );

	if ( this->mapSize.x < 1500 || this->mapSize.y < 1500
	||	 this->mapSize.x > 2500 || this->mapSize.y > 2500 )
	{
		throw std::logic_error("Bad size of map (min: 1500, max: 2500)");
	}

	this->dynLibs = parameters + 2;

	for (int i = 0; this->dynLibs[i]; i++)
		for (int j = i + 1; this->dynLibs[j]; j++)
			if ( strcmp(this->dynLibs[i], this->dynLibs[j]) == 0 )
				throw std::logic_error("The same dynamic libraries");
}

int 	Controller::handleCursor()
{
	switch (gui->getch())
	{
		case up: gui->playSound(0);
			this->cursor > 0 ? this->cursor -= 1 : 0; break;

		case down: gui->playSound(0);
			this->cursor < 2 ? this->cursor += 1 : 0; break;

		case enter: gui->playSound(1);
			return this->cursor == 0 ? -3 : -this->cursor;

		case cross: return -2;

		case 1:	return this->numOfLib != 1 ? 1 : 0;
		case 2:	return this->numOfLib != 2 && this->countLibs >= 2 ? 2 : 0;
		case 3: return this->numOfLib != 3 && this->countLibs == 3 ? 3 : 0;
		default: break;
	}

	return 0;
}

void	Controller::reloadGui(int num)
{
	gui->closeWindow();
	delete this->gui;

	this->loadLib(num);
	gui->openWindow(this->mapSize.x, this->mapSize.y);
	this->numOfLib = num;
}

void	Controller::loadLib(int numLib)
{
	IGui*		(*getGui)(int);

	if (this->dl_handle != NULL) {
		dlclose(this->dl_handle);
	}
	this->dl_handle = dlopen(this->dynLibs[numLib - 1], RTLD_LAZY | RTLD_LOCAL);
	if (!this->dl_handle)
		throw std::logic_error( dlerror() );

	getGui = (IGui*(*)(int)) dlsym(this->dl_handle, "getGui");
	if (!getGui) {
		throw std::logic_error( dlerror()) ;
	}
	this->gui = getGui(this->blockSize);
}
