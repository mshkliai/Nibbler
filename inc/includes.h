# ifndef INCLUDES_H
# define INCLUDES_H

# include <iostream>
# include <fstream>
# include <vector>
# include <time.h>
# include <dlfcn.h>
# include <regex>

# define esc 27
# define left 71
# define right 72
# define up 73
# define down 74
# define space 57
# define Pause 15
# define enter 58
# define cross -1

typedef struct 	s_cord
{
	int 		x, y;
	bool 		operator == (s_cord const &cords);
	void		operator =  (s_cord const &cords);
}				t_cord;

#endif