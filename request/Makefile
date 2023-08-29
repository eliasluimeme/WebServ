SRCS = ParseRequest.cpp request.cpp ../utils/string.cpp ../utils/sort_list.cpp
FLAGS = -Wextra -Werror -Wall
INCLUDES = includes
OBJS = $(SRCS:.cpp = .o)
NAME = webserv
CC = g++

all : 	$(NAME)
$(NAME):	$(OBJS)
				$(CC) $(FLAGS) -o $(NAME) $(OBJS)

clean :
			rm -rf $(NAME)
