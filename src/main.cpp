#include <iostream>

#include "../inc/Controller.hpp"
#include "../inc/Snake.hpp"
#include "../inc/Multiplayer.hpp"


int main(int ac, char **av)
{

	if (ac < 4 || ac > 6)
	{
		std::cerr << "Usage: ./snake [width] [heigth] dyn[1-3].so" << std::endl;
		return 0;
	}
	else
	{
		try
		{
			Controller *nibbler = new Controller(av + 1, ac - 3);
			nibbler->menuGame();

			delete nibbler;
		}
		catch (std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	return 0;
}
