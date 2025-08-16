#pragma once
#include "shape_presenter.h"
#include "shapes/line.h"
//#include "../views/line_view.h"

class LineView;
class LinePresenter : public ShapePresenter {
public:
    LinePresenter(std::shared_ptr<LineView> v, std::shared_ptr<Line> s);
    
    void startTransform(const Point& start) override;
    Point getStart() const;
    Point getEnd() const;
    void doTransform(const Point& p) override;

private:
    enum class Transform { None, Move, MoveBegin, MoveEnd };
    Transform getTransform(const Point& p) const;
    
    Transform m_currentTransform{Transform::None};
};