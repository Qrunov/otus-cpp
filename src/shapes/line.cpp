#include "shapes/line.h"
#include <cassert>

Line::Line(Point p) : m_start(p) {
    m_end = {p.x + 100, p.y + 100};
}

bool Line::isInside(const Point&) const {
    // Simplified implementation - line is always "inside"
    return true;
}

std::ostream& Line::operator<<(std::ostream& out) const {
    out << static_cast<uint32_t>(type()) << " ";
    out << m_start.x << " " << m_start.y << " ";
    out << m_end.x << " " << m_end.y << " ";
    return Shape::operator<<(out);
}

std::istream& Line::operator>>(std::istream& in) {
    in >> m_start.x >> m_start.y;
    in >> m_end.x >> m_end.y;
    return Shape::operator>>(in);
}

void Line::moveBeginOn(const Point& p) {
    m_start += p;
}

void Line::moveEndOn(const Point& p) {
    m_end += p;
}