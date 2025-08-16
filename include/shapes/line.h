#pragma once
#include "shape.h"

class Line : public Shape {
public:
    Line() = default;
    explicit Line(Point p);
    
    bool isInside(const Point&) const final;
    ShapeType type() const final { return ShapeType::Line; }
    
    std::ostream& operator<<(std::ostream& out) const final;
    std::istream& operator>>(std::istream& in) final;
    
    Point getStart() const { return m_start; }
    Point getEnd() const { return m_end; }
    void setStart(const Point& s) { m_start = s; }
    void setEnd(const Point& e) { m_end = e; }
    void moveBeginOn(const Point& p);
    void moveEndOn(const Point& p);

private:
    Point m_start;
    Point m_end;
};