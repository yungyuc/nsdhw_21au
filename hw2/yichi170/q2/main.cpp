#include <pybind11/pybind11.h>

namespace py = pybind11;

struct myvector {
	myvector(double x, double y): X{x}, Y{y} {}
	double X, Y;
};

double angle(myvector &a, myvector &b) {
	double length_a = sqrt(a.X * a.X + a.Y * a.Y);
	double length_b = sqrt(b.X * b.X + b.Y * b.Y);

	if (!length_a || !length_b)
		throw std::invalid_argument("vector's length is zero.");

	double cos_theta = (a.X * b.X + a.Y * b.Y) / (length_a * length_b);

	return acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

	py::class_<myvector>(m, "myvector")
		.def(py::init<double, double>())
		.def_readwrite("X", &myvector::X)
		.def_readwrite("Y", &myvector::Y);
								    
	m.def("angle", &angle, "calculates the angle (in radians) between two vectors");
}
