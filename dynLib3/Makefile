NAME = dyn3.so

SRC = OpenGLGui.cpp

all: $(NAME)

$(NAME):
	g++ -shared -fPIC -o $(NAME) $(SRC) -I$(HOME)/.brew/include/ -I$(HOME)/.brew/include/freetype2/ -L ~/.brew/lib -lglfw -lftgl -lfreetype -lSOIL -rpath ~/.brew/lib -framework OpenGL -framework Appkit
	mv $(NAME) ..
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f ../$(NAME)

re: fclean all
