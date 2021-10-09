CXX = g++ 
CXXFLAGS = -Wall -Wextra -std=c++17

SRC_EXT := cpp 
SRC_DIR := src

TARGET  := line 
TAR_DIR := target

SOURCES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT)) 
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(TAR_DIR)/%.o, $(SOURCES))

.PHONY: run check clear

$(TAR_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

$(TAR_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	$(TAR_DIR)/$(TARGET)

check:
	$(TAR_DIR)/$(TARGET) > result.txt

clean:
	-rm $(TAR_DIR)/* 

# for debug 
print-%:
	@echo '$*=$($*)'
