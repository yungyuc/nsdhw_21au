#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


Matrix multiply_naive(Matrix const & a, Matrix const & b)
{

    Matrix ans(a.nrow(), b.ncol());


    for (size_t i = 0; i < a.nrow(); ++i) {
        for (size_t j = 0; j < b.ncol(); ++j) {
            double v = 0;
            for (size_t k = 0; k < b.nrow(); ++k) {
                v += a(i, k) * b(k, j);
            }
            ans(i, j) = v;
        }

    }

    return ans;

}

Matrix multiply_tile(Matrix const & a, Matrix const & b, size_t tile)
{

    size_t bound_i, bound_j, bound_k;
    Matrix ans(a.nrow(), b.ncol());

    for (size_t i = 0; i < a.nrow(); i += tile)
    {
        for (size_t j = 0; j < b.ncol(); j += tile) 
        {
            bound_i = std::min(a.nrow(), i + tile);
            bound_j = std::min(b.ncol(), j + tile);
            for (size_t k = 0; k < a.ncol(); k += tile)
            {
                bound_k = std::min(a.ncol(), k + tile);

                for (size_t t_k = k; t_k < bound_k; t_k++)
                {
                    for (size_t t_i = i; t_i < bound_i; t_i++)
                    {
                        for (size_t t_j = j; t_j < bound_j; t_j++)
                        {
                            ans(t_i, t_j) += a(t_i, t_k) * b(t_k, t_j);
                        }
                    }
                }
            }
        }
    }
    return ans;

}

Matrix multiply_mkl(Matrix const & a, Matrix const & b)
{

    Matrix ans = Matrix(a.nrow(), b.ncol());

    cblas_dgemm(
         CblasRowMajor
        ,CblasNoTrans
        ,CblasNoTrans
        ,a.nrow()
        ,b.ncol()
        ,a.ncol()
        ,1.0
        ,a.get_matrix_buffer()
        ,a.ncol()
        ,b.get_matrix_buffer()
        ,b.ncol()
        ,0.0
        ,ans.get_matrix_buffer()
        ,ans.ncol()
    );

    return ans;
} 



PYBIND11_MODULE(_matrix,m){

    m.doc()="Matrix multiply";
	m.def("multiply_naive",&multiply_naive);
	m.def("multiply_tile",&multiply_tile);
	m.def("multiply_mkl",&multiply_mkl);
	pybind11::class_<Matrix>(m,"Matrix")
		.def(pybind11::init<const size_t, const size_t>())

        .def("__eq__", &Matrix::operator==)
        .def("__getitem__",[](const Matrix & m,std::array<int,2>index){return m(index[0],index[1]);})
        .def("__setitem__",[](Matrix & m, std::array<int,2>index,double value){m(index[0],index[1])=value;})

        .def_property_readonly("nrow",&Matrix::nrow)
        .def_property_readonly("ncol",&Matrix::nrow);

}