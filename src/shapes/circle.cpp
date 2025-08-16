#include "shapes/circle.h"
#include <cmath>

Circle::Circle(Point p) : center(p), radius(100) {}

bool Circle::isInside(const Point& p) const {
    double dx = p.x - center.x;
    double dy = p.y - center.y;
    return (dx*dx + dy*dy) <= (radius * radius);
}

std::ostream& Circle::operator<<(std::ostream& out) const {
    out << static_cast<uint32_t>(type()) << " ";
    out << center.x << " " << center.y << " ";
    out << radius << " ";
    return Shape::operator<<(out);
}

std::istream& Circle::operator>>(std::istream& in) {
    in >> center.x >> center.y;
    in >> radius;
    return Shape::operator>>(in);
}