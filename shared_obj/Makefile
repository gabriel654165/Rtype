##
## EPITECH PROJECT, 2021
## B-OOP-400-PAR-4-1-arcade-arthur.fouquet
## File description:
## Makefile
##

## USEFUL DEF ########################

CC			=	g++
CCL			=	clang++
RM			=	-rm -rf

EXT			=	.cpp

BULLET_BIN	=	shared_object.so

#######################################


## Dir  ###############################

BULLET_DIR		=	./Bullet

#######################################


## Files ##############################

SRC				=	$(addsuffix $(EXT), ./AComponents) \
					../ecs/Components/**

BULLET_FILE		=	$(addsuffix $(EXT), $(BULLET_DIR)/Bullet)

#######################################

## OBJ ################################

OBJ				=	$(SRC:.cpp=.o)

OBJ_BULLET		=	$(BULLET_FILE:.cpp=.o)

#######################################


## FLAGS ##############################

CXXFLAGS	=	-Wall -Wextra -fPIC -std=c++17

LDFLAGS		=	-shared

#######################################


## RULES ##############################

all:	$(OBJ) $(BULLET_BIN)

$(BULLET_BIN): LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
$(BULLET_BIN): CPPFLAGS += -I$(BULLET_DIR) -I$(../ecs/Components/)
$(BULLET_BIN): $(OBJ_BULLET)
	$(CC) -o $(BULLET_BIN) $(OBJ) $(OBJ_BULLET) $(CXXFLAGS) $(LDFLAGS)
	mv $(BULLET_BIN) ../build/lib/

clean:
	$(RM) $(OBJ)
	$(RM) $(OBJ_BULLET)

fclean:	clean
	$(RM) ../build/lib/$(BULLET_BIN)

re:	fclean all

.PHONY:	all clean fclean