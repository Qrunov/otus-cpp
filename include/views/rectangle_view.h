#pragma once
#include "shape_view.h"
#include "../presenters/rectangle_presenter.h"

class RectangleView : public ShapeView {
public:
    void updateView() const final;
};