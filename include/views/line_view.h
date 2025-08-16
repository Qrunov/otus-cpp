#pragma once
#include "shape_view.h"
#include "../presenters/line_presenter.h"

class LineView : public ShapeView {
public:
    void updateView() const final;
};