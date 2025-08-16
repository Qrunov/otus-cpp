#include "views/line_view.h"
#include "presenters/line_presenter.h"
#include <iostream>

void LineView::updateView() const {
    auto presenter = std::dynamic_pointer_cast<LinePresenter>(getPresenter());
    if (!presenter) return;

    Point start = presenter->getStart();
    Point end = presenter->getEnd();
    
    std::cout << "Drawing Line from (" << start.x << "," << start.y 
              << ") to (" << end.x << "," << end.y << ")";
    
    if (isActive()) {
        std::cout << " [ACTIVE]";
    }
    std::cout << std::endl;
}