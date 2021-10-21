#include <vector>
#include "line.h"


Line::Line() {}

Line::Line(Line const &data) {
    this->m_data = data.m_data;
}

Line::Line(Line &&data) {
    this->m_data = data.m_data;
}

Line::Line(size_t size_val) {
    this->m_data = std::vector<float>(size_val*2);
}

Line::~Line() {}

Line& Line::operator=(Line const &data) {
    this->m_data = data.m_data;
    return *this;
}

Line& Line::operator=(Line &&data) {
    this->m_data = data.m_data;
    return *this;
}

size_t Line::size() const {
    return this->m_data.size() / 2u;
}

float & Line::x(size_t it) {
    return this->m_data[it*2];
}

float & Line::y(size_t it) {
    return this->m_data[it*2+1];
}
