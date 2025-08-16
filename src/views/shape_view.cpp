#include "views/shape_view.h"
#include "presenters/shape_presenter.h"

void ShapeView::setPresenter(std::shared_ptr<ShapePresenter> presenter) {
    m_presenter = presenter;
}

void ShapeView::activate() {
    m_isActive = true;
    updateView();
}

void ShapeView::deactivate() {
    m_isActive = false;
    updateView();
}

