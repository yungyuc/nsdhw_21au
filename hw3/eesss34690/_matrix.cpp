#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <tuple>

#include <mkl.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <functional>

namespace py=pybind11;
using namespace std;

struct Matrix
{
public:
	Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
	{
		reset_buffer(nrow, ncol);
	}

	Matrix(Matrix const & other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
	{
		reset_buffer(other.m_nrow, other.m_ncol);
		memcpy(m_buffer, other.m_buffer, sizeof m_buffer);
	}

	Matrix & operator=(Matrix const & other)
	{
		if (this == &other) { return *this; }
		if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
		{
			throw out_of_range("number of elements mismatch");
		}
		memcpy(m_buffer, other.m_buffer, sizeof m_buffer);
		return *this;
	}

	Matrix(Matrix && other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
	{
		reset_buffer(0, 0);
		swap(m_nrow, other.m_nrow);
		swap(m_ncol, other.m_ncol);
		swap(m_buffer, other.m_buffer);
	}

	Matrix & operator=(Matrix && other)
	{
		if (this == &other) { return * this; }
		reset_buffer(0, 0);
		swap(m_nrow, other.m_nrow);
		swap(m_ncol, other.m_ncol);
		swap(m_buffer, other.m_buffer);
		return * this;
	}

	~Matrix()
	{
		reset_buffer(0, 0);
	}

	double   operator() (const size_t row, const size_t col) const { return m_buffer[index(row, col)]; }
	double & operator() (const size_t row, const size_t col)       { return m_buffer[index(row, col)]; }

	double   buffer(const size_t idx) const { return m_buffer[idx]; }
	double & buffer(const size_t idx)       { return m_buffer[idx]; }

	size_t nrow() const { return m_nrow; }
	size_t ncol() const { return m_ncol; }

	void initial(const double v) {for (size_t i=0; i<m_nrow*m_ncol; ++i) { m_buffer[i] = v; } }

public:

	size_t index(size_t const row, size_t const col) const
	{
		if ( m_nrow<row || m_ncol<col) { throw out_of_range("Index out of range"); }
		return row * m_ncol + col;
	}

	void reset_buffer(const size_t nrow,const size_t ncol)
	{
		if (m_buffer) { delete[] m_buffer; }
		const size_t nelement = nrow * ncol;
		if (nelement) { m_buffer = new double[nelement]; }
		else          { m_buffer = nullptr; }
		m_nrow = nrow;
		m_ncol = ncol;
	}

	size_t m_nrow = 0;
	size_t m_ncol = 0;
	double * m_buffer = nullptr;
	// delete unnessary variables
};

bool operator== (Matrix const & mat1, Matrix const & mat2)
{
    if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol()))
    {
        return false;
    }
    for (size_t i=0; i<mat1.nrow(); ++i)
    {
        for (size_t j=0; j<mat1.ncol(); ++j)
        {
            if (mat1.m_buffer[i, j] != mat2.m_buffer[i, j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!= (Matrix const & mat1, Matrix const & mat2)
{
    return !(mat1 == mat2);
}

void validate_multiplication(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

/*
 *  Direct naive matrix multiplication.
 */
Matrix multiply_naive(const Matrix & mat1, const Matrix & mat2)
{
	//std::cout<<"muliply_naive start"<<std::endl;

	validate_multiplication(mat1, mat2);

	Matrix ret(mat1.nrow(), mat2.ncol());

	const size_t nrow1 = mat1.nrow();
	const size_t ncol1 = mat1.ncol();
	const size_t ncol2 = mat2.ncol();

	for (size_t i=0; i<nrow1; ++i)
	{
		const size_t base1 = i * ncol1;
		for (size_t j=0; j<ncol2; ++j)
		{
			double v = 0;
			for (size_t k=0; k<ncol1; ++k){
				v += mat1.buffer(base1 + k) * mat2.buffer(k*ncol2 + j);
			}
			ret.buffer(base1 + j) = v;
		}
	}
	return ret;
}

/*
 *  Tiled matrix matrix multiplication.
 */
void process_tile(Matrix * ret, const Matrix & mat1, const Matrix & mat2, const size_t ncol1, const size_t nrow2, const size_t ncol2,
		  const size_t i_tsize, const size_t j_tsize, const size_t k_tsize, const size_t itsize, const size_t jtsize, const size_t ktsize)
{
	for (size_t ti=0; ti<itsize; ++ti)
	{
		const size_t ibase = (i_tsize + ti)*ncol1 + k_tsize;
		for (size_t tj=0; tj<jtsize; ++tj)
		{
			const size_t jbase = (j_tsize + tj)*nrow2 + k_tsize;
			const size_t ret_ij = (i_tsize + ti)*ncol2 + j_tsize + tj;
			double v = 0;
			for (size_t tk=0; tk<ktsize; ++tk)
			{
				//std::cout<<ret_ij<<" "<<ibase+tk<<" "<<jbase+tk<<std::endl;
				v += mat1.buffer(ibase + tk)*mat2.buffer(jbase + tk);
			}
			(*ret).buffer(ret_ij) += v;
		}
	}
}

Matrix * multiply_tile(const Matrix & mat1, const Matrix & mat2, const size_t bck_size)
{
	validate_multiplication(mat1, mat2);

	if (bck_size <= 0 || mat1.nrow()<=bck_size || mat1.ncol()<=bck_size || mat2.ncol()<=bck_size) { return multiply_naive(mat1, mat2); }

	Matrix ret(mat1.nrow(), mat2.ncol());
	ret.initial(0.0);

	const size_t nrow1 = mat1.nrow();
	const size_t ncol1 = mat1.ncol();
	const size_t nrow2 = mat2.nrow();
	const size_t ncol2 = mat2.ncol();

	const size_t ntrow1 = nrow1/tsize;
	const size_t ntcol2 = ncol2/tsize;
	const size_t ntcol1 = ncol1/tsize;

	const size_t rem_nrow1 = nrow1 % tsize;
	const size_t rem_ncol2 = ncol2 % tsize;
	const size_t rem_ncol1 = ncol1 % tsize;

	// rowmajor to colmajor
	double * col_buf = nullptr;
	//std::cout<<"multiply_itle rowmajor to colmajor"<<std::endl;
	for (size_t i=0; i<nrow2; ++i)
	{
		for (size_t j=0; j<ncol2; ++j)
		{
			colmat2.buffer(j*nrow2 + i) = mat2.buffer(i*ncol2 + j);
		}
	}

	//std::cout<<"multiply_itle multiply"<<std::endl;
	for (size_t i=0; i<ntrow1; ++i)
	{
		for (size_t j=0; j<ntcol2; ++j)
		{
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, tsize, tsize);

			}
			if(remncol1)
			{
				const size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, tsize, remncol1);
			}
		}
		if (remncol2)
		{
			const size_t j = ntcol2;
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, remncol2, tsize);
			}
			if(remncol1)
			{
				size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, remncol2, remncol1);
			}
		}

	}
	if (remnrow1)
	{
		const size_t i = ntrow1;
		for (size_t j=0; j<ntcol2; ++j)
		{
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, tsize, tsize);

			}
			if(remncol1)
			{
				const size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, tsize, remncol1);
			}
		}
		if (remncol2)
		{
			const size_t j = ntcol2;
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, remncol2, tsize);
			}
			if(remncol1)
			{
				size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, remncol2, remncol1);
			}
		}
	}

	return ret;
}


/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix * multiply_mkl(const Matrix & mat1, const Matrix & mat2)
{
	//std::cout<<"multiply_mkl start"<<std::endl;

	mkl_set_num_threads(1);

	Matrix ret(mat1.nrow(), mat2.ncol());

	cblas_dgemm(
		CblasRowMajor,
		CblasNoTrans,
		CblasNoTrans,
		mat1.nrow(),
		mat2.ncol(),
		mat1.ncol(),
		1.0,
		mat1.m_buffer,
		mat1.ncol(),
		mat2.m_buffer,
		mat2.ncol(),
		0.0,
		ret.m_buffer,
		ret.ncol()
		);
	return ret;
}

PYBIND11_MODULE(_matrix, m)
{
	m.doc() = "pybind11 matrix multiplication test";
	m.def("multiply_naive", & multiply_naive, "naive method");
	m.def("multiply_tile", & multiply_tile, "Tiling solution");
	m.def("multiply_mkl", & multiply_mkl, "use mkl");
	py::class_<Matrix>(m, "Matrix")
		.def(py::init<const size_t, const size_t>())
		.def_property_readonly("nrow", &Matrix::nrow)
		.def_property_readonly("ncol", &Matrix::ncol)
		.def("__getitem__", [](const Matrix & mat, tuple<size_t, size_t> t) -> double
		{
			//std::cout<<"getitem:start"<<std::endl;
			return mat(get<0>(t), get<1>(t));
		})
		.def("__setitem__", [](Matrix & mat, tuple<size_t, size_t> t, const double & v)
		{
			//std::cout<<"setitem:start"<<std::endl;
			mat(get<0>(t), get<1>(t)) = v;
		})
		.def("__eq__", &Matrix::operator==)
		;
}
