#pragma once
#include <iostream>
#include <cmath>
#include "constants.h"

struct Point {
    int x;
    int y;
    
    Point& operator+=(const Point& p);
    Point operator+(const Point& p) const;
    Point operator-(const Point& p) const;
    bool isBeside(const Point& p) const;
    
    friend std::istream& operator>>(std::istream& in, Point& p);
    friend std::ostream& operator<<(std::ostream& out, const Point& p);
};

// Inline implementations for simple operations
inline Point& Point::operator+=(const Point& p) {
    x += p.x;
    y += p.y;
    return *this;
}

inline Point Point::operator+(const Point& p) const {
    return {x + p.x, y + p.y};
}

inline Point Point::operator-(const Point& p) const {
    return {x - p.x, y - p.y};
}

inline bool Point::isBeside(const Point& p) const {
    return (std::hypot(p.x - x, p.y - y) < Constants::GAP);
}