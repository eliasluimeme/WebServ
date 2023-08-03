NAME = webserv

SRC	= ./Srcs/main.cpp ./Srcs/Client.cpp ./Srcs/Parser.cpp ./Srcs/Server.cpp

OBJ	= $(SRC:.cpp=.o)

FLAGS = -std=c++98 -fsanitize=address -g3 #-Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(SRC)
	c++ $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(OBJ)
				
fclean:	clean
	rm -rf $(NAME)

re:	fclean all