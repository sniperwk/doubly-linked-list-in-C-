TARGET		:= main

SRC_DIR		:= src

OBJ_DIR		:= obj

BIN_DIR		:= bin

INC_DIR 	:= include

EXT		:= cpp

CXX 		:= g++-5

CXXFLAGS	:= -g -Wall -std=c++14 -I $(INC_DIR)

SOURCES 	:= $(wildcard $(SRC_DIR)/*.$(EXT))

OBJECTS 	:= $(SOURCES:$(SRC_DIR)/%.$(EXT)=$(OBJ_DIR)/%.o)

DEBUG_FLAGS	:= --tool=memcheck --leak-check=yes

DEPS 		:= $(OBJECTS:.o=.d)

.PHONY: all clean default

-include $(DEPS)

all: default

default: $(BIN_DIR)/$(TARGET)
	@./$(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(OBJECTS) -g -Wall -o $@

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/$(TARGET)

debug: $(BIN_DIR)/$(TARGET)
	valgrind $(DEBUG_FLAGS) $(BIN_DIR)/$(TARGET)
