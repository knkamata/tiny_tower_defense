NAME		= ttd
CC			= clang++
CFLAGS		= -std=c++11 -Wall
SDL2		= -I./include/SDL2  -L./lib/SDL2 -lSDL2
SDL2IMAGE	= -I./include/SDL2_image -L./lib/SDL2_image -lSDL2_image

SRC		= \
		  Actor.cpp \
		  AIComponent.cpp \
		  AIState.cpp \
		  Bullet.cpp \
		  CircleComponent.cpp \
		  Component.cpp \
		  Enemy.cpp \
		  Game.cpp \
		  Grid.cpp \
		  Main.cpp \
		  Math.cpp \
		  MoveComponent.cpp \
		  NavComponent.cpp \
		  Search.cpp \
		  SpriteComponent.cpp \
		  Tile.cpp \
		  Tower.cpp \

.PHONY: all run clean re

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SDL2) $(SDL2IMAGE) $(SRC) -o $(NAME)

run:
	./$(NAME)

clean:
	$(RM) $(NAME)

re: clean all
