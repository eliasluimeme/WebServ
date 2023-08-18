NAME = webserv

SRC	= ./Srcs/main.cpp ./Srcs/Client.cpp ./Srcs/Parser.cpp ./Srcs/Server.cpp ./Srcs/Request.cpp ./Srcs/Response.cpp

OBJ	= $(SRC:.cpp=.o)

FLAGS = -std=c++98 -fsanitize=address -g3 -lz #-Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(SRC)
	@c++ $(FLAGS) $(SRC) -o $(NAME)

clean:
	@rm -rf $(OBJ)
				
fclean:	clean
	@rm -rf $(NAME)
	@rm -rf request*

re:	fclean all