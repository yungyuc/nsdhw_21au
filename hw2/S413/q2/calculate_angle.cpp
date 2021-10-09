#include<pybind11/pybind11.h>
#include<pybind11/stl.h>

#include<vector>
#include<cmath>

namespace py = pybind11;

float calculate_angle(std::vector<float> &a, std::vector<float> &b){
	try{
	
		if(a.size() != b.size()){
			throw ("Vectors must have same dimensions");
		}

		size_t dim = a.size();

		if(dim == 0){
			throw ("Vectors are dimension 0.");
		}

		float dot = 0;
		float magA = 0;
		float magB = 0;
		for(size_t i=0; i<dim; ++i){
	      		dot += (a[i]*b[i]);
	      		magA += (a[i]*a[i]);
	      		magB += (b[i]*b[i]);
		}

		magA = std::sqrt(magA);
		magB = std::sqrt(magB);

		if( (magA == 0) || (magB == 0) ){
			throw ("One or both vector's magnitudes is 0.");
		}

		return acos(dot/(magA*magB));
	}
	catch(const char* msg){
		std::cerr << msg << std::endl;
		return -1;
	}

}

PYBIND11_MODULE(_vector, m){
	m.doc() = "pybind11 plugin";
	m.def("calculate_angle", &calculate_angle);
}
