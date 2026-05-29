NAME = game_of_life

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./src
LIBS = -lncurses

SRC_DIR = src
MOD_DIR = $(SRC_DIR)/modules
INIT_DIR = $(SRC_DIR)/init_states
BUILD_DIR = build

MAIN = $(SRC_DIR)/main.c
MODULES = $(wildcard $(MOD_DIR)/*.c)

SRC = $(MAIN) $(MODULES)

OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)


all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/$(NAME): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


run: $(BUILD_DIR)/$(NAME)
	./$(BUILD_DIR)/$(NAME) < $(INIT_DIR)/map1.txt

run-map: $(BUILD_DIR)/$(NAME)
	@if [ -z "$(MAP)" ]; then \
		echo "Usage: make run-map MAP=map6.txt"; \
		echo "Available maps:"; \
		ls $(INIT_DIR); \
		exit 1; \
	fi
	./$(BUILD_DIR)/$(NAME) < $(INIT_DIR)/$(MAP)


clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(BUILD_DIR)/$(NAME)

re: fclean all


format:
	clang-format -n $(SRC) $(MOD_DIR)/*.c

format-fix:
	clang-format -i $(SRC) $(MOD_DIR)/*.c


maps:
	@echo "Available maps:"
	@ls $(INIT_DIR)

.PHONY: all clean fclean re run run-map maps format format-fix