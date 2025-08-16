#include "shapes/shape_factory.h"
#include "shapes/line.h"
#include "shapes/circle.h"
#include "shapes/rectangle.h"

std::shared_ptr<Shape> ShapeFactory::makeDefaultLine(const Point& where) {
    return std::make_shared<Line>(where);
}

std::shared_ptr<Shape> ShapeFactory::makeDefaultCircle(const Point& where) {
    return std::make_shared<Circle>(where);
}

std::shared_ptr<Shape> ShapeFactory::makeDefaultRectangle(const Point& where) {
    return std::make_shared<Rectangle>(where);
}

std::shared_ptr<Shape> ShapeFactory::makeShapeForType(ShapeType type) {
    switch(type) {
        case ShapeType::Line:
            return std::make_shared<Line>();
        case ShapeType::Circle:
            return std::make_shared<Circle>();
        case ShapeType::Rectangle:
            return std::make_shared<Rectangle>();
        default:
            return nullptr;
    }
}