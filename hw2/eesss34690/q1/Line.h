# include <vector>
class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line() = default;
    size_t size() const;
    float x(size_t it) const;
    float & x(size_t it);
    float y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
    std::vector<float> m_x;
    std::vector<float> m_y;
    size_t m_size;
};
