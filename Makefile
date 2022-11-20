NAME = ft_traceroute

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRC = main.c \
	  network.c \
	  packet.c \
	  trace.c \
	  utils.c

HEADER = traceroute.h

OBJ := $(SRC:%.c=./$(OBJ_DIR)/%.o)
SRC := $(SRC:%=./$(SRC_DIR)/%)

HEADER := $(HEADER:%=./$(INC_DIR)/%)

FLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(FLAGS) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE)
