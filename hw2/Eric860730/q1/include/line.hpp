#pragma once

#include <vector>

struct Point {
  float x;
  float y;
};

class Line {
public:
  // Custom constructor.
  Line(size_t size);
  // Basic constructors.
  Line(const Line &other);            // copy constructor.
  Line &operator=(const Line &other); // copy assignment operator.
  // Destructor.
  ~Line() = default;
  // Accessors.
  size_t size() const;

  const float &x(size_t idx) const; // getter
  float &x(size_t idx);             // setter

  const float &y(size_t idx) const; // getter
  float &y(size_t idx);             // setter

private:
  // Member data.
  std::vector<Point> m_coordinate; // memory buffer for the points.
};                                 /* end class Line */
