NAME		= lite

TEST_NAME	= test

COMPILER	= g++

CFLAGS		= -Wall -Wextra -Werror -O3 -std=c++17

SRC			= src/main.cpp \
			  src/Data.cpp \
			  src/utils.cpp \
			  src/SDL_utils.cpp

OBJDIR		= obj

OBJ			= $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC))

ifeq ($(OS), Windows_NT)
    SDL_FLAGS	= -IC:/msys64/mingw64/include/SDL2
	SDL_LIBS = -LC:/msys64/mingw64/lib -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lSDL2 
else
    SDL_FLAGS = $(shell sdl2-config --cflags)
    SDL_LIBS  = -lSDL2_mixer -lSDL2_ttf -lSDL2_image $(shell sdl2-config --libs)
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILER) $(CFLAGS) $(OBJ) -o $(NAME) $(SDL_FLAGS) $(SDL_LIBS)

%.o: %.cpp
	$(COMPILER) $(CFLAGS) $(SDL_FLAGS) -c $< -o $@

# Build the test executable
$(TEST_NAME): test.cpp
	$(COMPILER) $(CFLAGS) test.cpp -o $(TEST_NAME) $(SDL_FLAGS) $(SDL_LIBS)

clean:
	rm -f $(OBJ)

# fclean now explicitly targets both the main name and the test name
fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re
