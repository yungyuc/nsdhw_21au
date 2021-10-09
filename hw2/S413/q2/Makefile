build:
	g++ -O3 -Wall -shared -std=c++11 -fPIC $$(python3 -m pybind11 --includes) calculate_angle.cpp -o _vector$$(python3-config --extension-suffix) -I/usr/include/python3.8

test:
	python3 -m pytest -v
	
clean:
	rm *.so
	rm -rf __pycache__
