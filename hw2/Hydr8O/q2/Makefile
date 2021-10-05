.PHONY: clean, run, check
SOURCE_FILE_NAME = _vector
FILE_NAME = $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix)

FILE_NAME:
	if [ ! -d "./pybind11" ]; then git clone https://github.com/pybind/pybind11 ; fi
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -I ./pybind11/include $(SOURCE_FILE_NAME).cpp -o $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix)

clean:
	rm -rf pybind11 *.so

run:
	make

check:
	./main > results.txt

	