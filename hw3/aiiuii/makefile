all:
	g++ -shared -O3 -std=c++11 -fPIC \
	`python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` \
	-I/opt/intel/mkl/include -I/opt/intel/mkl/lib/intel64 -I/opt/intel/lib/intel64 \
	-L/opt/intel/mkl/lib \
	-I /opt/intel/compilers_and_libraries/linux/mkl/include -L /opt/intel/compilers_and_libraries/linux/mkl/lib \
	-L /opt/intel/compilers_and_libraries/linux/lib -liomp5 -lblas -lpthread
test: all
	python3 -m pytest unit_test.py
clean:
	rm -f *.so 