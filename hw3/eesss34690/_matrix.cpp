#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <utility>

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

	Matrix(Matrix  & other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
	{
		reset_buffer(other.m_nrow, other.m_ncol);
		memcpy(m_buffer, other.m_buffer, sizeof m_buffer);
	}

	Matrix & operator=(Matrix  & other)
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

	Matrix()
	{
		reset_buffer(0, 0);
	}
	~Matrix()
	{
		reset_buffer(0, 0);
	}

	double & operator() ( size_t row,  size_t col)   { return m_buffer[index(row, col)]; }

	double & buffer( size_t idx)       { return m_buffer[idx]; }

	size_t nrow()  { return m_nrow; }
	size_t ncol()  { return m_ncol; }

	void initial( double v, size_t nrow, size_t ncol)
	{
		m_nrow = nrow;
		m_ncol = ncol;
		for (size_t i=0; i<m_nrow*m_ncol; ++i)
		{
			m_buffer[i] = v; 
		} 
	}
	Matrix transpose() 
	{
		Matrix ret(nrow(), ncol());

		for (size_t i=0; i<ret.nrow(); ++i)
		{
			for (size_t j=0; j<ret.ncol(); ++j)
			{
				ret(j, i) = (*this)(i, j);
			}
		}

		return ret;
	}

	bool operator== (Matrix  & mat2)
	{
		if ((ncol() != mat2.ncol()) && (nrow() != mat2.ncol()))
		{
			return false;
		}
		for (size_t i=0; i<nrow(); ++i)
		{
			for (size_t j=0; j<ncol(); ++j)
			{
				if ((*this)(i, j) != mat2(i, j))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool operator!= (Matrix  & mat2)
	{
		return !((*this) == mat2);
	}

public:

	size_t index(size_t  row, size_t  col) 
	{
		if ( m_nrow<row || m_ncol<col) { throw out_of_range("Index out of range"); }
		return row * m_ncol + col;
	}

	void reset_buffer( size_t nrow, size_t ncol)
	{
		if (m_buffer) { delete[] m_buffer; }
		 size_t nelement = nrow * ncol;
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


class multiply_tile
{
public:
	Matrix mat1;
	Matrix ret;
	Matrix trans_mat2;
	size_t bck_tile;
	size_t nrow1;
	size_t ncol1;
	size_t nrow2;
	size_t ncol2;

	size_t bck_row1;
	size_t bck_col2;
	size_t bck_col1;

	size_t rem_row1;
	size_t rem_col2;
	size_t rem_col1;
public:
	multiply_tile( Matrix & in_1,  Matrix & in_2, size_t block_size)
	{
		validate_multiplication(in_1, in_2);
		mat1 = in_1;
		trans_mat2 = in_2.transpose();

		nrow1 = mat1.nrow();
		ncol1 = mat1.ncol();
		nrow2 = in_2.nrow();
		ncol2 = in_2.ncol();
		bck_tile = block_size;

		bck_row1 = nrow1 / bck_tile;
		bck_col2 = ncol2 / bck_tile;
		bck_col1 = ncol1 / bck_tile;

		rem_row1 = nrow1 % bck_tile;
		rem_col2 = ncol2 % bck_tile;
		rem_col1 = ncol1 % bck_tile;

		ret.initial(0.0, mat1.nrow(), in_2.ncol());
	}
	void validate_multiplication( Matrix & in_1,  Matrix & in_2)
	{
		if (in_1.ncol() != in_2.nrow())
		{
			throw out_of_range(
				"the number of first matrix column "
				"differs from that of second matrix row");
		}
	}
	void block_op( size_t i_tsize,  size_t j_tsize,  size_t k_tsize, 
		 size_t itsize,  size_t jtsize,  size_t ktsize)
	{
		for (size_t ti=0; ti<itsize; ++ti)
		{
			 size_t ibase = (i_tsize + ti)*ncol1 + k_tsize;
			for (size_t tj=0; tj<jtsize; ++tj)
			{
				 size_t jbase = (j_tsize + tj)*nrow2 + k_tsize;
				 size_t ret_ij = (i_tsize + ti)*ncol2 + j_tsize + tj;
				double v = 0;
				for (size_t tk=0; tk<ktsize; ++tk)
				{
					v += mat1.buffer(ibase + tk)*trans_mat2.buffer(jbase + tk);
				}
				ret.buffer(ret_ij) += v;
			}
		}
	}
	Matrix multiply()
	{
		for (size_t i=0; i<bck_row1; ++i)
		{
			for (size_t j=0; j<bck_col2; ++j)
			{
				for (size_t k=0; k<bck_col1; ++k)
				{
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, bck_tile, bck_tile, bck_tile);

				}
				if(rem_col1)
				{
					 size_t k = bck_col1;
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, bck_tile, bck_tile, bck_tile);
				}
			}
			if (rem_col2)
			{
				 size_t j = bck_col2;
				for (size_t k=0; k<bck_col1; ++k)
				{
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, bck_tile, rem_col2, bck_tile);
				}
				if(rem_col1)
				{
					size_t k = bck_col1;
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, bck_tile, rem_col2, rem_col1);
				}
			}

		}
		if (rem_row1)
		{
			size_t i = bck_row1;
			for (size_t j=0; j<bck_col2; ++j)
			{
				for (size_t k=0; k<bck_col1; ++k)
				{
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, rem_row1, bck_tile, bck_tile);

				}
				if(rem_col1)
				{
					 size_t k = bck_col1;
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, rem_row1, bck_tile, rem_col1);
				}
			}
			if (rem_col2)
			{
				 size_t j = bck_col2;
				for (size_t k=0; k<bck_col1; ++k)
				{
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, rem_row1, rem_col2, bck_tile);
				}
				if(rem_col1)
				{
					size_t k = bck_col1;
					block_op(i*bck_tile, j*bck_tile, k*bck_tile, rem_row1, rem_col2, rem_col1);
				}
			}
		}
		return ret;
	}

};

Matrix multiply_naive( Matrix & mat1,  Matrix & mat2)
{
	//std::cout<<"muliply_naive start"<<std::endl;

	if (mat1.ncol() != mat2.nrow())
		{
			throw out_of_range(
				"the number of first matrix column "
				"differs from that of second matrix row");
		}

	Matrix ret(mat1.nrow(), mat2.ncol());

	 size_t nrow1 = mat1.nrow();
	 size_t ncol1 = mat1.ncol();
	 size_t ncol2 = mat2.ncol();

	for (size_t i=0; i<nrow1; ++i)
	{
		 size_t base1 = i * ncol1;
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
 * Use MKL for the matrix matrix multiplication.
 */
/*
Matrix * multiply_mkl( Matrix & mat1,  Matrix & mat2)
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
*/
PYBIND11_MODULE(_matrix, m)
{
	m.doc() = "pybind11 matrix multiplication test";
	m.def("multiply_naive", & multiply_naive, "naive method");
	//m.def("multiply_mkl", & multiply_mkl, "use mkl");
	py::class_<Matrix>(m, "Matrix")
		.def(py::init< size_t,  size_t>())
		.def(py::init<>())
		.def_property_readonly("nrow", &Matrix::nrow)
		.def_property_readonly("ncol", &Matrix::ncol)
		.def("__getitem__", [](const Matrix & mat, size_t idx) -> double
		{
			return mat.m_buffer[idx];
		})
		.def("__setitem__", [](Matrix & mat, size_t idx, const double & v)
		{
			mat.m_buffer[idx] = v;
		})
		.def("__eq__", &Matrix::operator==);
		;
	py::class_<multiply_tile>(m, "multiply_tile")
		.def(py::init<Matrix & ,  Matrix & , size_t>())
		.def("multiply", &multiply_tile::multiply)
	;
}
