CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall -Wextra
TARGET=main.out

.PHONY:clean all
all: $(TARGET)


$(TARGET): Mat.cpp main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET)
