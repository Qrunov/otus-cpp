#include "presenters/line_presenter.h"
#include <algorithm>
#include <views/shape_view.h>
#include <views/line_view.h>

LinePresenter::LinePresenter(std::shared_ptr<LineView> v,
                           std::shared_ptr<Line> s) 
    : ShapePresenter(v,s) {}

Point LinePresenter::getStart() const {
    return std::dynamic_pointer_cast<Line>(m_model)->getStart();
}

Point LinePresenter::getEnd() const {
    return std::dynamic_pointer_cast<Line>(m_model)->getEnd();
}

void LinePresenter::doTransform(const Point& p) {
    if (m_currentTransform == Transform::None) return;
    
    auto line = std::dynamic_pointer_cast<Line>(m_model);
    Point delta = p - m_startTransformPoint;
    
    switch(m_currentTransform) {
        case Transform::MoveBegin:
            line->moveBeginOn(delta);
            break;
        case Transform::MoveEnd:
            line->moveEndOn(delta);
            break;
        case Transform::Move:
            line->moveBeginOn(delta);
            line->moveEndOn(delta);
            break;
        default: break;
    }
    
    m_startTransformPoint = p;
    m_view->updateView();
}

void LinePresenter::startTransform(const Point& start) {
    ShapePresenter::startTransform(start);
    m_currentTransform = getTransform(start);
}

LinePresenter::Transform LinePresenter::getTransform(const Point& p) const {
    auto line = std::dynamic_pointer_cast<Line>(m_model);
    if (p.isBeside(line->getStart())) return Transform::MoveBegin;
    if (p.isBeside(line->getEnd())) return Transform::MoveEnd;
    return Transform::Move;
}