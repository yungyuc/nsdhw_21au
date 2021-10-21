# include "Line.hpp"

Line::Line(){
    _pdata = new std::vector<Point>();
}

Line::Line(Line const &item){
    _pdata = new std::vector<Point>(item.size());
    *(_pdata) = *(item._pdata);
}

Line::Line(size_t size){
    _pdata = new std::vector<Point>(size);
}

Line & Line::operator=(Line const &item){
    delete _pdata;
    _pdata = new std::vector<Point>(item.size());
    *(_pdata) = *(item._pdata);
    return *this;
}

Line::~Line(){
    delete _pdata;
}

size_t Line::size() const{
    return _pdata->size();
}

float & Line::x(size_t it) const{
    return _pdata->at(it).x;
}

float & Line::x(size_t it){
    return _pdata->at(it).x;
}

float & Line::y(size_t it) const{
    return _pdata->at(it).y;
}

float & Line::y(size_t it){
    return _pdata->at(it).y;
}