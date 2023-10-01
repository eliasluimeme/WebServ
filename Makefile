NAME = webserv

SRC	= ./Srcs/main.cpp ./Srcs/Client.cpp ./Srcs/Server.cpp ./Srcs/Request.cpp ./Srcs/Response.cpp ./Srcs/CGI.cpp ./conf/config.cpp ./conf/ConfigServer.cpp ./conf/HelperFunc.cpp ./conf/ReaderConfig.cpp 

OBJ	= $(SRC:.cpp=.o)

FLAGS = -std=c++98 -g3 -lz -fsanitize=address -Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(SRC)
	@c++ $(FLAGS) $(SRC) -o $(NAME)

clean:
	@rm -rf $(OBJ)
				
fclean:	clean
	@rm -rf $(NAME)
	@rm -rf request* cgi* out_*

re:	fclean all