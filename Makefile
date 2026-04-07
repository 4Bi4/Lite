NAME		= lite
TEST_NAME	= test
COMPILER	= g++
CFLAGS		= -Wall -Wextra -Werror -std=c++17

COMPILER	= g++

CFLAGS		= -Wall -Wextra -Werror -O3 -std=c++17

SRC			= src/main.cpp \
			  src/Data.cpp \
			  src/utils.cpp \
			  src/SDL_utils.cpp

OBJDIR		= obj

OBJ			= $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC))

SDL_FLAGS	= $(shell sdl2-config --cflags)

SDL_LIBS	= -lSDL2_mixer -lSDL2_ttf -lSDL2_image $(shell sdl2-config --libs)

# It is safer to put LDFLAGS at the end of the compilation command
SDL_FLAGS	= -IC:/msys64/mingw64/include/SDL2
SDL_LIBS = -LC:/msys64/mingw64/lib -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lSDL2 
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

#g++ -Wall -Wextra -std=c++17 test.cpp -o test \
$(sdl2-config --cflags --libs) -lSDL2_mixer -lSDL2_ttf -lSDL2_image