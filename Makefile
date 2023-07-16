TARGET:=demo
CC:=gcc
CFLAGS:=-std=c99 -Wall -Wextra -pedantic
LFLAG:=ncurses
INCLUDE:=./include/
SRC:=./src
BUILD:=./build

all: clean $(BUILD)/$(TARGET)

.PHONY: all clean

$(BUILD)/$(TARGET): $(SRC)/$(TARGET).c $(SRC)/dungeon_generator_bsp.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $^ -l$(LFLAG)

clean: 
	@rm -rf ./build/demo
