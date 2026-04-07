# COLORS
GREEN   := \033[1;32m
RED     := \033[1;31m
MAGENTA := \033[1;35m
BLUE	:= \033[1;34m
FLASH   := \033[1;33;5m
NEGRITA := \033[1m
NC      := \033[0m

NAME		= lite

TEST_NAME	= test

COMPILER	= c++

CFLAGS		= -Wall -Wextra -Werror -O3 -std=c++17

SRC			= src/main.cpp \
			  src/Data.cpp \
			  src/utils.cpp \
			  src/SDL_utils.cpp

OBJDIR		= obj

OBJ			= $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC))

SDL_FLAGS	= $(shell sdl2-config --cflags)

SDL_LIBS	= -lSDL2_mixer -lSDL2_ttf -lSDL2_image $(shell sdl2-config --libs)

all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILER) $(CFLAGS) $(OBJ) -o $(NAME) $(SDL_FLAGS) $(SDL_LIBS)
	@echo "->$(GREEN) PROGRAM COMPILED SUCCESSFULLY!$(NC)"

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(SDL_FLAGS) -c $< -o $@

#	Build the test executable
$(TEST_NAME): test.cpp
	@echo "->$(BLUE) Compiling test...$(NC)"
	$(COMPILER) $(CFLAGS) test.cpp -o $(TEST_NAME) $(SDL_FLAGS) $(SDL_LIBS)
	@echo "->$(GREEN) TEST COMPILED SUCCESSFULLY!$(NC)"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(RED)removed object files$(NC)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(TEST_NAME)
	@echo "$(RED)removed executables$(NC)"
	@echo "->$(GREEN) [all clean]$(NC)"

re: fclean all

.PHONY: all clean fclean re test
