#include "presenters/rectangle_presenter.h"
#include "views/rectangle_view.h"


RectanglePresenter::RectanglePresenter(std::shared_ptr<RectangleView> v,
                                    std::shared_ptr<Rectangle> s)
    : ShapePresenter(v, s) {}

void RectanglePresenter::doTransform([[maybe_unused]]const Point& p) {
//    if (!isOnTransform()) return;
    
//    auto rect = std::dynamic_pointer_cast<Rectangle>(m_model);
//    Point delta = p - m_startTransformPoint;
//    rect->move(delta); // Предполагаем наличие метода move в Rectangle
    
//    m_startTransformPoint = p;
    m_view->updateView();
}