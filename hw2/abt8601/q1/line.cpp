#include "line.hpp"

#include <stdexcept>

Line::Line() = default;

Line::Line(Line const &) = default;

Line::Line(Line &&) = default;

Line & Line::operator=(Line const &) = default;

Line & Line::operator=(Line &&) = default;

Line::Line(size_t const size) : m_coord(size) {}

Line::~Line() = default;

size_t Line::size() const noexcept { return m_coord.size(); }

float const & Line::x(size_t const it) const
{
    check_range(it);
    return m_coord[it].first;
}

float & Line::x(size_t const it)
{
    check_range(it);
    return m_coord[it].first;
}

float const & Line::y(size_t const it) const
{
    check_range(it);
    return m_coord[it].second;
}

float & Line::y(size_t const it)
{
    check_range(it);
    return m_coord[it].second;
}

void Line::check_range(size_t const it) const
{
    if (it >= size())
    {
        throw std::out_of_range("Line index out of range");
    }
}
