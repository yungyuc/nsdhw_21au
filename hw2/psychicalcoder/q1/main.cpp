/*
  Copyright (C) 2019 Marvin Liu

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <memory>
#include <utility>

class Line
{
public:
  Line() = delete;
  Line(Line const & );
  Line(Line       &&);
  Line & operator=(Line const & );
  Line & operator=(Line       &&);
  Line(size_t size);
  ~Line() = default;
  size_t size() const;
  float & x(size_t it) const;
  float & x(size_t it);
  float & y(size_t it) const;
  float & y(size_t it);
private:
  std::unique_ptr<float[]> x_arr;
  std::unique_ptr<float[]> y_arr;
  size_t length;
    // Member data.
}; /* end class Line */


Line::Line(size_t size) {
  this->length = size;
  this->x_arr = std::make_unique<float[]>(size);
  this->y_arr = std::make_unique<float[]>(size);
}

Line::Line(Line const &b) {
  this->length = b.length;
  x_arr = std::make_unique<float[]>(b.length);
  y_arr = std::make_unique<float[]>(b.length);
  for (size_t i = 0; i < b.length; i++) {
      this->x_arr[i] = b.x_arr[i];
  }
  for (size_t i = 0; i < b.length; i++) {
    this->y_arr[i] = b.y_arr[i];
  }
}

Line::Line(Line &&b) {
  this->length = b.length;
  this->x_arr = std::move(b.x_arr);
  this->y_arr = std::move(b.y_arr);
  b.length = 0;
}


float &Line::x(size_t it) const {
  return this->x_arr[it];
}

float &Line::x(size_t it) {
  return this->x_arr[it];
}

float &Line::y(size_t it) const {
  return this->y_arr[it];
}

float &Line::y(size_t it) {
  return this->y_arr[it];
}

size_t Line::size() const {
  return this->length;
}

Line & Line::operator=(Line const &b) {
  if (this != &b) {
    this->length = b.length;
    this->x_arr.reset(new float[b.length]);
    this->y_arr.reset(new float[b.length]);
    for (size_t i = 0; i < b.length; i++) {
      this->x_arr[i] = b.x_arr[i];
    }
    for (size_t i = 0; i < b.length; i++) {
      this->y_arr[i] = b.y_arr[i];
    }
  }
  return *this;
}

Line & Line::operator=(Line &&b) {
  if (this != &b) {
    this->length = b.length;
    this->x_arr = std::move(b.x_arr);
    this->y_arr = std::move(b.y_arr);
    b.length = 0;
  }
  return *this;
}

int main(int, char **)
{
  Line line(3);
  line.x(0) = 0; line.y(0) = 1;
  line.x(1) = 1; line.y(1) = 3;
  line.x(2) = 2; line.y(2) = 5;
  
  Line line2(line);
  line2.x(0) = 9;
  
  std::cout << "line: number of points = " << line.size() << std::endl;
  for (size_t it=0; it<line.size(); ++it)
  {
    std::cout << "point " << it << ":"
              << " x = " << line.x(it)
              << " y = " << line.y(it) << std::endl;
  }
  
  std::cout << "line2: number of points = " << line.size() << std::endl;
  for (size_t it=0; it<line.size(); ++it)
  {
    std::cout << "point " << it << ":"
              << " x = " << line2.x(it)
              << " y = " << line2.y(it) << std::endl;
  }
  
  return 0;
}
