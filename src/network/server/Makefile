##
## R_TYPE PROJECT, 2024
## Makefile
## File description:
## Makefile
##

SRC = 	Server.cpp									\
		../handleData/HandleData.cpp 				\
		../../ecs/components/Components.cpp 		\

OBJ = $(SRC:.cpp=.o)

CXX = g++

CXXFLAGS = -fPIC -I .. -I ../handleData -I ../../../include -I ../../../lib -I ../../ecs -I ../../ecs/components -Wall -Wextra

LDFLAGS = -shared

LIBPATH = ../../../sharedLibraries/

NAME = server.so

all : $(NAME)

$(NAME):	$(OBJ)
	$(CXX) $(LDFLAGS) -o $(NAME) $(OBJ)
	mv $(NAME) $(LIBPATH)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(LIBPATH)/$(NAME)

re: fclean all

.PHONY : all clean fclean re
