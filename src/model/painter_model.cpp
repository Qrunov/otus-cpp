#include "model/painter_model.h"
#include "shapes/shape_factory.h"
#include <algorithm>
#include <sstream>

using namespace std;

shared_ptr<Shape> PainterModel::takeTopShapeOn(const Point& where) {
    auto it = find_if(shapes.begin(), shapes.end(),[&](auto &e){return e -> isInside(where);});
    shared_ptr<Shape> res;
    if (it != shapes.end())
    {
	shapes.push_back(*it);
	res = *it;
	shapes.erase(it);
    }
    return res;
}

void PainterModel::insertTop(shared_ptr<Shape> shape) {
    shapes.push_front(shape);
}

void PainterModel::removeTop() {
    if (!shapes.empty())
        shapes.pop_front();
}

void PainterModel::clear() {
    shapes.clear();
}

list<shared_ptr<Shape>> PainterModel::getShapes() const {
    return shapes;
}

ostream& operator<<(ostream& out, const PainterModel& model) {
    // Write header
    out.write(PainterModel::SIGN, sizeof(PainterModel::SIGN));
    out.write(reinterpret_cast<const char*>(&PainterModel::VERSION), sizeof(PainterModel::VERSION));
    out.write(reinterpret_cast<const char*>(&PainterModel::SUBVERSION), sizeof(PainterModel::SUBVERSION));

    // Write shapes
    for (const auto& shape : model.shapes) {
        shape -> operator<<(out);
    }

    return out;
}

istream& operator>>(istream& in, PainterModel& model) {
    // Read and verify header
    char sign[sizeof(PainterModel::SIGN)];
    in.read(sign, sizeof(PainterModel::SIGN));
    
    if (string(sign) != PainterModel::SIGN) {
        in.setstate(ios::failbit);
        return in;
    }

    uint32_t version, subversion;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    in.read(reinterpret_cast<char*>(&subversion), sizeof(subversion));

    model.clear();

    // Read shapes
    while (in.good()) {
        uint32_t type;
        if (!in.read(reinterpret_cast<char*>(&type), sizeof(type))) 
            break;

        auto shape = ShapeFactory::makeShapeForType(static_cast<ShapeType>(type));
        if (!shape) continue;

        shape->operator>>(in);
        model.shapes.push_back(shape);
    }

    return in;
}