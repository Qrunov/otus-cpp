#pragma once
#include "shape_presenter.h"
#include "shapes/circle.h"

class CircleView;
class CirclePresenter : public ShapePresenter {
public:
    CirclePresenter(std::shared_ptr<CircleView> v, std::shared_ptr<Circle> s);
    void doTransform(const Point& p) override;
};