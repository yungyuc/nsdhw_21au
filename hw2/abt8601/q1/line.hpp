#pragma once
#ifndef NSDHW_21AU_HW2Q1_LINE_HPP
#define NSDHW_21AU_HW2Q1_LINE_HPP

#include <cstddef>
#include <vector>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();

    size_t size() const noexcept;
    float const & x(size_t it) const;
    float       & x(size_t it);
    float const & y(size_t it) const;
    float       & y(size_t it);

private:
    void check_range(size_t it) const;

    std::vector<std::pair<float, float>> m_coord;
};

#endif
