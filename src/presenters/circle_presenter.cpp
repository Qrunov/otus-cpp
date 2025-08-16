#include "presenters/circle_presenter.h"
#include "views/circle_view.h"
#include "shapes/circle.h"

CirclePresenter::CirclePresenter(std::shared_ptr<CircleView> v,
                               std::shared_ptr<Circle> s)
    : ShapePresenter(v, s) {}

void CirclePresenter::doTransform([[maybe_unused]]const Point& p) {
//    if (!isOnTransform()) return;
    
//    auto circle = std::dynamic_pointer_cast<Circle>(m_model);
//    Point delta = p - m_startTransformPoint;

//    circle->move(delta); 
    m_view->updateView();
}