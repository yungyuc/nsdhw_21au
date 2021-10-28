#include "matrix.hpp"

// default constructor
Matrix::Matrix()
{
    m_nrow = 0;
    m_ncol = 0;
    m_buffer = nullptr;
}

// copy constructor
Matrix::Matrix(Matrix const &cp)
{
    m_nrow = cp.m_nrow;
    m_ncol = cp.m_ncol;

    //if(m_buffer)
    //    delete[] m_buffer;
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement];
    for(size_t i=0; i<nelement; i++)
        m_buffer[i] = cp.m_buffer[i];
}

// move constructor
Matrix::Matrix(Matrix &&mv)
{
    m_nrow = mv.m_nrow;
    m_ncol = mv.m_ncol;
    m_buffer = mv.m_buffer;
}

// copy assignment operator
Matrix & Matrix::operator=(Matrix const &cp_rval)
{
    if(this == &cp_rval)
        return *this;

    m_nrow = cp_rval.m_nrow;
    m_ncol = cp_rval.m_ncol;

    //if(m_buffer)
    //    delete[] m_buffer;
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement];
    for(size_t i=0; i<nelement; i++)
        m_buffer[i] = cp_rval.m_buffer[i];

    return *this;
}

// move assignment operator
Matrix & Matrix::operator=(Matrix &&mv_rval)
{
    if(this == &mv_rval)
        return *this;

    m_nrow = mv_rval.m_nrow;
    m_ncol = mv_rval.m_ncol;
    m_buffer = mv_rval.m_buffer;

    return *this;
}

// custom constructor
Matrix::Matrix(size_t nrow, size_t ncol)
{
    m_nrow = nrow;
    m_ncol = ncol;
    size_t nelement = nrow * ncol;
    m_buffer = new double[nelement];
}

// destructor
Matrix::~Matrix()
{
    if(m_buffer)
        delete[] m_buffer;
}

// access the number of rows
size_t Matrix::nrow() const
{
    return m_nrow;
}

// access the number of columns
size_t Matrix::ncol() const
{
    return m_ncol;
}

// element getter
double Matrix::operator() (size_t row, size_t col) const
{
    return m_buffer[row*m_ncol + col];
}

// element setter
double & Matrix::operator() (size_t row, size_t col)
{
    return m_buffer[row*m_ncol + col];
}

