#ifndef memallo_hpp
#define memallo_hpp

#include<cstdlib>
#include<new>
#include<memory>
#include<limits>
#include<atomic>
#include<vector>
#include<iostream>

struct ByteCounterImp{
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;
};

class ByteCounter{
public:
    
    ByteCounter(): m_imp(new ByteCounterImp){
        incref();
    }

    ByteCounter(ByteCounter const & other): m_imp(other.m_imp){
	incref();
    }

    ByteCounter & operator=(ByteCounter const & other){
	if(&other != this){
	    decref();
	    m_imp = other.m_imp;
	    incref();
	}

	return *this;
    }

    ByteCounter(ByteCounter && other): m_imp(other.m_imp){
	incref();
    }

    ByteCounter & operator=(ByteCounter && other){
	if(&other != this){
	    decref();
	    m_imp = other.m_imp;
	    incref();
	}

	return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other){
	std::swap(m_imp, other.m_imp);
    }

    void increase(std::size_t amount){
	m_imp->allocated += amount;
    }

    void decrease(std::size_t amount){
	m_imp->deallocated += amount;
    }

    std::size_t bytes() const { return m_imp->allocated - m_imp->deallocated; }
    std::size_t allocated() const { return m_imp->allocated; }
    std::size_t deallocated() const { return m_imp->deallocated; }
    std::size_t refcount() const { return m_imp->refcount; }

//private:

    void incref() { ++m_imp->refcount; }

    void decref(){
	if(nullptr == m_imp){
	    return;
	}
	else if(1 == m_imp->refcount){
	    delete m_imp;
	    m_imp = nullptr;
	}
	else{
	    --m_imp->refcount;
	}
    }

    ByteCounterImp* m_imp;
};

template <class T>
struct MyAllocator{

    using value_type = T;

    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator (const MyAllocator <U> & other) noexcept{
        counter = other.counter;
    }

    T* allocate(std::size_t n){
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T)){
	    throw std::bad_alloc();
	}

	const std::size_t bytes = n*sizeof(T);

	T* p = static_cast<T*>(std::malloc(bytes));

	if(p){
	    counter.increase(bytes);
	    return p;
	}
	else{
	    throw std::bad_alloc();
	}
    }

    void deallocate(T* p, std::size_t n) noexcept{
	std::free(p);

	const std::size_t bytes = n*sizeof(T);
	counter.decrease(bytes);
    }

    std::size_t bytes() const { return counter.bytes(); }
    std::size_t allocated() const { return counter.allocated(); }
    std::size_t deallocated() const { return counter.deallocated(); }

    ByteCounter counter;
};

#endif
