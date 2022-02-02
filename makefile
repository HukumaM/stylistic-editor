CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb3 -O0

BIN		:= bin
SRC		:= src
INCLUDE	:= include

EXECUTABLE	:= stylistic_editor

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@
 
clean:
	-rm $(BIN)/*
