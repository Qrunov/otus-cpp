#include "presenters/painter_presenter.h"
#include "presenters/presenter_factory.h"
#include "shapes/shape_factory.h"
#include "views/view_factory.h"
#include "views/painter_view.h"
#include <algorithm>

void PainterPresenter::action(const Point& where) {
    switch(m_state) {
        case State::Line:
            m_model->insertTop(ShapeFactory::makeDefaultLine(where));
            break;
        case State::Circle:
            m_model->insertTop(ShapeFactory::makeDefaultCircle(where));
            break;
        case State::Rectangle:
            m_model->insertTop(ShapeFactory::makeDefaultRectangle(where));
            break;
        case State::Select:
            if (!m_active || !m_active->isInside(where)) {
                auto shape = m_model->takeTopShapeOn(where);
                if (shape) activate(shape);
            }
            break;
    }
    update();
}

void PainterPresenter::activate(std::shared_ptr<Shape> selected) {
    auto view = makeShapeView(selected->type());
    m_active = makeShapePresenter(view, selected);
    view->activate();
}

void PainterPresenter::deleteActive() {
    if (m_active) {
        m_model->removeTop();
        m_active.reset();
        update();
    }
}

void PainterPresenter::beginNew() {
    m_active.reset();
    m_model->clear();
    update();
}

void PainterPresenter::save(std::ostream& out) const {
    out << *m_model;
}

void PainterPresenter::load(std::istream& in) {
    in >> *m_model;
    update();
}

void PainterPresenter::startTransform(const Point& where) {
    if (m_active) m_active->startTransform(where);
}

void PainterPresenter::endTransform() {
    if (m_active) m_active->endTransform();
}

void PainterPresenter::move(const Point& where) {
    if (m_active && m_active->isOnTransform()) {
        m_active->doTransform(where);
    }
}

void PainterPresenter::update() {
    m_view->updateView();
}