NAME = snake

SRC =	main.cpp \
		Multiplayer.cpp \
		Controller.cpp \
		Snake.cpp

S_SRC = $(addprefix ./src/, $(SRC))

OBJ = $(S_SRC:.cpp=.o)

CFLAGS = -Wall -Wextra -Werror

CC = g++

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	cd dynLib1 && make
	cd dynLib2 && make
	cd dynLib3 && make

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

install: fclean
	sh install.sh
	make

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)
	cd dynLib1 && make fclean
	cd dynLib2 && make fclean
	cd dynLib3 && make fclean

re: fclean all
