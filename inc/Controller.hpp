# ifndef CONTROLLER_H
# define CONTROLLER_H

# include "IGui.hpp"
# include "Snake.hpp"
# include "Multiplayer.hpp"

# include "includes.h"

class	Controller
{
	public:
	
		Controller();
		Controller(char **parameters, int countLibs);
		Controller(Controller const &obj);
		~Controller();

		Controller 		&operator = (Controller const &);
		
		void			menuGame();

	private:

		t_cord			mapSize;
		int				blockSize;
		int				cursor;

		Snake 			*snake;

		char			**dynLibs;
		int 			countLibs;
		int 			numOfLib;
		void			*dl_handle;
		IGui 			*gui;
		
		void			calcMap();

		void			parseParameters(char **parameters);
		int				handleCursor();
		void			reloadGui(int num);
		void			loadLib(int numLib);

};

#endif