NAME = dyn2.so

SRC = SDL2Gui.cpp

all: $(NAME)

$(NAME):
	g++ -shared -fPIC -o $(NAME) $(SRC) -I ~/.brew/include -L ~/.brew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -rpath ~/.brew/lib
	mv $(NAME) ..
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f ../$(NAME)

re: fclean all
