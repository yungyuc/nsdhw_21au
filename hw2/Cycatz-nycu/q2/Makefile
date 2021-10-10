CXX = g++ 
# CXXFLAGS = -fPIC -Wall -Wextra -std=c++11 $(shell python3 -m pybind11 --includes)
CXXFLAGS = -fPIC -Wall -Wextra -std=c++11 -I /usr/include/python3.8
LD_FLAGS = -shared

SRC_EXT := cpp 
SRC_DIR := src

TARGET  := _vector$(shell python3-config --extension-suffix)
TAR_DIR := .

SOURCES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT)) 
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(TAR_DIR)/%.o, $(SOURCES))

TEST_SCRIPT := test_vector.py

.PHONY: run check clear

$(TAR_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(LD_FLAGS) -o $@ $^

$(TAR_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TAR_DIR)/$(TARGET) $(TEST_SCRIPT)
	pytest -v  

clean:
	-rm -rf .pytest_cache .mypy_cache __pycache__ $(TAR_DIR)/$(TARGET) $(OBJECTS) 

# for debug 
print-%:
	@echo '$*=$($*)'
