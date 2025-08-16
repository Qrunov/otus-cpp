#pragma once
#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle() = default;
    explicit Rectangle(Point p);
    
    bool isInside(const Point&) const final;
    ShapeType type() const final { return ShapeType::Rectangle; }
    
    std::ostream& operator<<(std::ostream& out) const final;
    std::istream& operator>>(std::istream& in) final;

private:
    Point downLeftCorner;
    double m_h{150.0};
    double m_v{100.0};
    bool isTransformMode{false};
};