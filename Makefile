# --- COLORS ---
GREEN   := \033[1;32m
RED     := \033[1;31m
MAGENTA := \033[1;35m
BLUE    := \033[1;34m
NC      := \033[0m

NAME        = lite
TEST_NAME   = test
COMPILER    = g++
CFLAGS      = -O3 -std=c++20 -Wall -Wextra -Werror
OBJDIR      = obj
SRCDIR      = src

VENDOR_DIR  = vendor
V_INCLUDE   = -I$(VENDOR_DIR)/include

ifeq ($(OS), Windows_NT)
    PLATFORM    = windows
    BIN_NAME    = $(NAME).exe
    SDL_LIBS    = -L$(VENDOR_DIR)/lib/$(PLATFORM) -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
else
    PLATFORM    = linux
    BIN_NAME    = $(NAME)
    SDL_LIBS    = -L$(VENDOR_DIR)/lib/$(PLATFORM) \
                  -Wl,-rpath,'$$ORIGIN/$(VENDOR_DIR)/lib/$(PLATFORM)' \
                  -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
endif

SRC         = $(SRCDIR)/main.cpp \
              $(SRCDIR)/Data.cpp \
              $(SRCDIR)/utils.cpp \
			  $(SRCDIR)/SDL_utils.cpp \
			  $(SRCDIR)/graphicsUtils.cpp

OBJ         = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

all: $(BIN_NAME)

$(BIN_NAME): $(OBJ)
	$(COMPILER) $(CFLAGS) $(OBJ) -o $(BIN_NAME) $(SDL_LIBS)
	@echo "-> $(GREEN)PROGRAM COMPILED SUCCESSFULLY!$(NC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(V_INCLUDE) -c $< -o $@

$(TEST_NAME): test.cpp
	$(COMPILER) $(CFLAGS) test.cpp -o $(TEST_NAME) $(V_INCLUDE) $(SDL_LIBS)
	@echo "-> $(GREEN)TEST COMPILED SUCCESSFULLY!$(NC)"

clean:
	@rm -rf $(OBJDIR)
	@echo "$(RED)removed object files and directory$(NC)"

fclean: clean
	@rm -f $(BIN_NAME)
	@rm -f $(TEST_NAME)
	@echo "$(RED)removed executables$(NC)"
	@echo "-> $(GREEN)[all clean]$(NC)"

re: fclean all

.PHONY: all clean fclean re test