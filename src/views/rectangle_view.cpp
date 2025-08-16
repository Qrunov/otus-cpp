#include "views/rectangle_view.h"
#include "presenters/rectangle_presenter.h"
#include <iostream>

void RectangleView::updateView() const {
    auto model = getPresenter()->getModel();
    auto rect = std::dynamic_pointer_cast<Rectangle>(model);
    if (!rect) return;

    std::cout << "Drawing Rectangle";
    
    if (isActive()) {
        std::cout << " [ACTIVE]";
    }
    std::cout << std::endl;
}