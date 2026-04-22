# --- COLOR DEFINITON ---
ifeq ($(shell tput colors 2>/dev/null),)
	GREEN   :=
	RED     :=
	MAGENTA :=
	BLUE    :=
	NC      :=
else
	BOLD    := $(shell tput bold)
	RESET   := $(shell tput sgr0)

	GREEN   := $(BOLD)$(shell tput setaf 2)
	RED     := $(BOLD)$(shell tput setaf 1)
	MAGENTA := $(BOLD)$(shell tput setaf 5)
	BLUE    := $(BOLD)$(shell tput setaf 4)
	NC      := $(RESET)
endif

NAME        = lite

TEST_NAME   = test

COMPILER    = g++

CFLAGS      = -O3 -std=c++20 -Wall -Wextra -Werror

OBJDIR      = obj

SRCDIR      = src

VENDOR_DIR  = vendor

V_INCLUDE   = -I$(VENDOR_DIR)/include

DLL_DIR     = $(VENDOR_DIR)/lib/$(PLATFORM)

ifeq ($(OS),Windows_NT)
    PLATFORM    = windows
    BIN_NAME    = $(NAME).exe
    SDL_LIBS    = -L$(VENDOR_DIR)/lib/$(PLATFORM) -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
    COPY_DLLS   = cp $(DLL_DIR)/*.dll .
else
    PLATFORM    = linux
    BIN_NAME    = $(NAME)
    SDL_LIBS    = -L$(VENDOR_DIR)/lib/$(PLATFORM) \
                  -Wl,-rpath,'$$ORIGIN/$(VENDOR_DIR)/lib/$(PLATFORM)' \
                  -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
	COPY_DLLS   = # NO DLLS TO COPY ON LINUX
endif

SRC         = $(SRCDIR)/main.cpp \
              $(SRCDIR)/Data.cpp \
              $(SRCDIR)/utils.cpp \
			  $(SRCDIR)/Player.cpp \
              $(SRCDIR)/SDL_utils.cpp \
			  $(SRCDIR)/graphicsUtils.cpp

OBJ			= $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

all: $(BIN_NAME)

$(BIN_NAME): $(OBJ)
	$(COMPILER) $(CFLAGS) $(OBJ) -o $(BIN_NAME) $(SDL_LIBS)
	$(COPY_DLLS)
	@printf "%b""-> $(GREEN)PROGRAM COMPILED SUCCESSFULLY!$(NC)\n"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(V_INCLUDE) -c $< -o $@

$(TEST_NAME): test.cpp
	$(COMPILER) $(CFLAGS) test.cpp -o $(TEST_NAME) $(V_INCLUDE) $(SDL_LIBS)
	$(COPY_DLLS)
	@printf "%b""-> $(GREEN)TEST COMPILED SUCCESSFULLY!$(NC)\n"

clean:
	@rm -rf $(OBJDIR)
	@printf "%b""$(RED)removed object files and directory$(NC)\n"

fclean: clean
	@rm -f $(BIN_NAME)
	@rm -f $(TEST_NAME)
	@rm -f *.dll
	@printf "%b""$(RED)removed executables$(NC)\n"
	@printf "%b""-> $(BLUE)[all clean]$(NC)\n"

re: fclean all

.PHONY: all clean fclean re test