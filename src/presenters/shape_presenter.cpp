#include "presenters/shape_presenter.h"
#include "views/shape_view.h"

ShapePresenter::ShapePresenter(std::shared_ptr<ShapeView> v, 
                             std::shared_ptr<Shape> m) 
    : m_view(v), m_model(m) {
    //m_view->setPresenter(shared_from_this());
}

ShapePresenter::~ShapePresenter() {
    m_view->deactivate();
}

void ShapePresenter::update() {
    m_view->updateView();
}

void ShapePresenter::activate() {
    m_view->activate();
}

bool ShapePresenter::isInside(Point where) {
    return m_model->isInside(where);
}

void ShapePresenter::startTransform(const Point& start) {
    m_startTransformPoint = start;
    m_onTransform = true;
}

void ShapePresenter::endTransform() {
    m_onTransform = false;
}