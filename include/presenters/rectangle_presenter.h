#pragma once
#include "shape_presenter.h"
#include "shapes/rectangle.h"
//#include "../views/rectangle_view.h"

class RectangleView;
class RectanglePresenter : public ShapePresenter {
public:
    RectanglePresenter(std::shared_ptr<RectangleView> v, std::shared_ptr<Rectangle> s);
    void doTransform(const Point& p) override;
};