#include "views/circle_view.h"
#include "presenters/circle_presenter.h"
#include <iostream>

void CircleView::updateView() const {
    auto model = getPresenter()->getModel();
    auto circle = std::dynamic_pointer_cast<Circle>(model);
    if (!circle) return;

    std::cout << "Drawing Circle";
    
    if (isActive()) {
        std::cout << " [ACTIVE]";
    }
    std::cout << std::endl;
}