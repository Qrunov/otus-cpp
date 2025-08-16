#pragma once
#include <memory>
std::shared_ptr<ShapePresenter>	makeShapePresenter(std::shared_ptr<ShapeView> view,std::shared_ptr<Shape> shape);
