#include "alloc.hpp"

// default constructor
ByteCounter::ByteCounter()
{
    m_impl = new ByteCounterImpl;
    incref();
}

// copy constructor
ByteCounter::ByteCounter(ByteCounter const &other)
{
    m_impl = other.m_impl;
    incref();
}

// move constructor
ByteCounter::ByteCounter(ByteCounter &&other)
{
    m_impl = other.m_impl;
    incref();
}

// copy assignment constructor
ByteCounter & ByteCounter::operator=(ByteCounter const &other)
{
    if(&other != this)
    {
        decref();
        m_impl = other.m_impl;
        incref();
    }
    return *this;
}

// move assignment constructor
ByteCounter & ByteCounter::operator=(ByteCounter &&other)
{
    if(&other != this)
    {
        decref();
        m_impl = other.m_impl;
        incref();
    }
    return *this;
}

// destructor
ByteCounter::~ByteCounter()
{
    decref();
}

void ByteCounter::swap(ByteCounter &other)
{
    ByteCounterImpl *temp = m_impl;
    m_impl = other.m_impl;
    other.m_impl = temp;
}

void ByteCounter::increase(size_t amount)
{
    m_impl->allocated += amount;
}

void ByteCounter::decrease(size_t amount)
{
    m_impl->deallocated += amount;
}

size_t ByteCounter::bytes() const
{
    return m_impl->allocated - m_impl->deallocated;
}

size_t ByteCounter::allocated() const
{
    return m_impl->allocated;
}

size_t ByteCounter::deallocated() const
{
    return m_impl->deallocated;
}

/* This is for debugging. */
size_t ByteCounter::refcount() const
{
    return m_impl->refcount;
}

void ByteCounter::incref()
{
    ++m_impl->refcount;
}

void ByteCounter::decref()
{
    if(m_impl == nullptr)
    {
        // Do nothing.
    }
    else if(m_impl->refcount == 1)
    {
        delete m_impl;
        m_impl = nullptr;
    }
    else
        --m_impl->refcount;
}

