#include "shapes/rectangle.h"

Rectangle::Rectangle(Point p) : downLeftCorner(p), m_h(150), m_v(100) {}

bool Rectangle::isInside(const Point& p) const {
    return (p.x >= downLeftCorner.x) && 
           (p.x <= downLeftCorner.x + m_h) &&
           (p.y >= downLeftCorner.y) && 
           (p.y <= downLeftCorner.y + m_v);
}

std::ostream& Rectangle::operator<<(std::ostream& out) const {
    out << static_cast<uint32_t>(type()) << " ";
    out << downLeftCorner.x << " " << downLeftCorner.y << " ";
    out << m_h << " " << m_v << " ";
    return Shape::operator<<(out);
}

std::istream& Rectangle::operator>>(std::istream& in) {
    in >> downLeftCorner.x >> downLeftCorner.y;
    in >> m_h >> m_v;
    return Shape::operator>>(in);
}