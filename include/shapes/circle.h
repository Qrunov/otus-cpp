#pragma once
#include "shape.h"

class Circle : public Shape {
public:
    Circle() = default;
    explicit Circle(Point p);
    
    bool isInside(const Point&) const final;
    ShapeType type() const final { return ShapeType::Circle; }
    
    std::ostream& operator<<(std::ostream& out) const final;
    std::istream& operator>>(std::istream& in) final;

private:
    Point center;
    double radius{100.0};
};