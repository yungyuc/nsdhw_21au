CXX := g++
CXXFLAGS := -std=c++11
WARNINGFLAGS := -Wall -Wextra
SRC_DIRS := ./
TARGET := main
SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
TXT := result.txt

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGFLAGS) -c $< -o $@

run:
	./main
check:
	./main > $(TXT)
clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS) $(TXT)
