##
## R_TYPE PROJECT, 2024
## r-type
## File description:
## Makefile
##

CXX = g++

CXXFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -g -Wall -Wextra
CPPFLAGS = -std=c++20 -iquote include -iquote lib -iquote src/games 							\
			-iquote src/runGame -iquote src/ecs -iquote src/ecs/components 						\
			-iquote src/ecs/systems	-iquote src/network/server -iquote src/network/client		\

SRC = 	src/runGame/RunGame.cpp											\
		src/network/server/Server.cpp									\
		src/network//client/Client.cpp									\
		src/ecs/components/Components.cpp								\
    	src/ecs/systems/CollisionSystem.cpp								\
    	src/ecs/systems/ControlSystem.cpp								\
    	src/ecs/systems/DrawSystem.cpp									\
    	src/ecs/systems/EnemyFiringSystem.cpp							\
    	src/ecs/systems/EnemySpawnSystem.cpp							\
    	src/ecs/systems/FiringSystem.cpp								\
    	src/ecs/systems/KillOffscreenEntitiesSystem.cpp					\
    	src/ecs/systems/LifeSystem.cpp									\
		src/ecs/systems/PlayerSpawnSystem.cpp 							\
    	src/ecs/systems/PositionSystem.cpp								\
    	src/ecs/systems/SoundSystem.cpp									\
    	src/ecs/systems/SpriteAnimationSystem.cpp						\
    	src/ecs/systems/MusicSystem.cpp									\
    	lib/Lib.cpp														\


SRC_SERVER = 	src/games/RtypeGameServer.cpp							\
    			src/network//server/main.cpp							\

SRC_CLIENT = 	src/games/RtypeGameClient.cpp 							\
    			src/network//client/main.cpp							\

OBJ = $(SRC:.cpp=.o)
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)

SERVER_NAME = rtype_server
CLIENT_NAME = rtype_client


all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(OBJ) $(OBJ_SERVER)
	$(CXX) $(CXXFLAGS) -o $(SERVER_NAME) $(OBJ) $(OBJ_SERVER)

$(CLIENT_NAME): $(OBJ) $(OBJ_CLIENT)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_NAME) $(OBJ) $(OBJ_CLIENT)

clean:
	$(RM) $(OBJ) $(OBJ_SERVER) $(OBJ_CLIENT)

fclean: clean
	$(RM) $(SERVER_NAME) $(CLIENT_NAME)

re: fclean all

format:
	find . -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i

.PHONY: all clean fclean re
