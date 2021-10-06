#include "line.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>

/* Initializer list */
Line::Line(size_t size) : m_coordinate(size) {}

/* Define the copy constructor */
Line::Line(const Line &other) : m_coordinate(other.m_coordinate) {}

/* Define the copy assignment operator */
Line &Line::operator=(const Line &other) {
  if (this == &other) {
    return *this;
  }

  m_coordinate = other.m_coordinate;
  return *this;
}

/* Define Accessors */
size_t Line::size() const { return m_coordinate.size(); }

const float &Line::x(size_t idx) const { return m_coordinate.at(idx).x; }

float &Line::x(size_t idx) { return m_coordinate.at(idx).x; }

const float &Line::y(size_t idx) const { return m_coordinate.at(idx).y; }

float &Line::y(size_t idx) { return m_coordinate.at(idx).y; }
