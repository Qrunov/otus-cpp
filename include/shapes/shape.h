#pragma once
#include "common/enums.h"
#include "common/point.h"
#include <iostream>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    virtual bool isInside(const Point&) const = 0;
    virtual ShapeType type() const = 0;
    
    virtual std::istream& operator>>(std::istream& in);
    virtual std::ostream& operator<<(std::ostream& out) const;
    
    Color getFillColor() const { return fillColor; }
    Color getBorderColor() const { return borderColor; }
    void setFillColor(Color color) { fillColor = color; }
    void setBorderColor(Color color) { borderColor = color; }

protected:
    Color fillColor{Color::White};
    Color borderColor{Color::Black};
};