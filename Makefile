CC = gcc
CFLAGS := -g -Og -Wall

LD = gcc
CFLAGS += -I /opt/homebrew/Cellar/sdl2/2.28.4/include
LDFLAGS += -L /opt/homebrew/Cellar/sdl2/2.28.4/lib/ -l SDL2
 
SRC_DIR = src
BIN_DIR = bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))


.PHONY: all build clean run debug

all: build
build: $(BIN_DIR)/executable

$(BIN_DIR)/executable: $(OBJ_FILES)
	$(LD) $(LDFLAGS) $^ -o $@

$(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_FILES): | $(BIN_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -r $(BIN_DIR)
run: build
	./bin/executable
debug:
	lldb ./bin/executable
