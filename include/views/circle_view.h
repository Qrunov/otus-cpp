#pragma once
#include "shape_view.h"
#include "../presenters/circle_presenter.h"

class CircleView : public ShapeView {
public:
    void updateView() const final;
};