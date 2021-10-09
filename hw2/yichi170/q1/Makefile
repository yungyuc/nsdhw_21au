CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -O3

exe = main
objs = *.o
txt = result.txt

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

$(exe): $(objs)
# 	@ECHO *.o
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean run check

clean:
	rm -f $(objs) $(exe)

check: $(TARGET)
	./$(exe) > $(txt)

run: $(TARGET)
	./$(exe)
