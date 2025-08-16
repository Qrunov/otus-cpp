#include "shapes/shape.h"
#include <type_traits>

std::istream& Shape::operator>>(std::istream& in) {
    std::underlying_type_t<Color> color;
    in >> color;
    fillColor = static_cast<Color>(color);
    in >> color;
    borderColor = static_cast<Color>(color);
    return in;
}

std::ostream& Shape::operator<<(std::ostream& out) const {
    out << static_cast<std::underlying_type_t<Color>>(fillColor) << " ";
    out << static_cast<std::underlying_type_t<Color>>(borderColor) << " ";
    return out;
}