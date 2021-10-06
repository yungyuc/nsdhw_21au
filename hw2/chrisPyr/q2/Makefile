CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -fPIC -shared
EXEC = testadd.py
PYTHON = python3
SRC = angle

all:$(SRC).*.so

$(SRC).*.so: $(SRC).cpp
	@$(CC) $(CFLAGS) `$(PYTHON) -m pybind11 --includes` $< -o _vector`$(PYTHON)-config --extension-suffix`

.PHONY:test
test: all
	$(PYTHON) -m pytest -v

.PHONY:clean
clean:
	$(RM) -r *.so __pycache__/
