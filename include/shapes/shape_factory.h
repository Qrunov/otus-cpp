#pragma once
#include "shape.h"
#include <memory>

class ShapeFactory {
public:
    static std::shared_ptr<Shape> makeDefaultLine(const Point& where);
    static std::shared_ptr<Shape> makeDefaultCircle(const Point& where);
    static std::shared_ptr<Shape> makeDefaultRectangle(const Point& where);
    static std::shared_ptr<Shape> makeShapeForType(ShapeType type);
};