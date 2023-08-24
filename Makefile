NAME = webserv

SRC	= ./Srcs/main.cpp ./Srcs/Client.cpp ./Srcs/Parser.cpp ./Srcs/Server.cpp ./Srcs/Request.cpp ./Srcs/Response.cpp ./conf/config.cpp ./conf/ConfigServer.cpp ./conf/HelperFunc.cpp ./conf/ReaderConfig.cpp

OBJ	= $(SRC:.cpp=.o)

FLAGS = -std=c++98  -g3 -lz #-Wall -Wextra -Werror -fsanitize=address

all: $(NAME)

$(NAME): $(SRC)
	@c++ $(FLAGS) $(SRC) -o $(NAME)

clean:
	@rm -rf $(OBJ)
				
fclean:	clean
	@rm -rf $(NAME)
	@rm -rf request* output.txt

re:	fclean all