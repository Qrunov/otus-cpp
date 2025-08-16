#pragma once
#include <list>
#include <memory>
#include <iostream>
#include "shapes/shape.h"
#include "common/point.h"

class PainterModel {
public:
    std::shared_ptr<Shape> takeTopShapeOn(const Point& where);
    void insertTop(std::shared_ptr<Shape> shape);
    void removeTop();
    void clear();
    std::list<std::shared_ptr<Shape>> getShapes() const;
    
    friend std::ostream& operator<<(std::ostream &out, const PainterModel &model);
    friend std::istream& operator>>(std::istream &in, PainterModel &model);

private:
    std::list<std::shared_ptr<Shape>> shapes;

    // Constants for serialization
    static constexpr const char* SIGN = "PAINTERX";
    static constexpr uint32_t VERSION = 1;
    static constexpr uint32_t SUBVERSION = 1;
};

// Stream operators declarations
std::ostream& operator<<(std::ostream &out, const PainterModel &model);
std::istream& operator>>(std::istream &in, PainterModel &model);