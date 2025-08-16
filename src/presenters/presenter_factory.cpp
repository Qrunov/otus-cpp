#include "presenters/circle_presenter.h"
#include "presenters/line_presenter.h"
#include "presenters/rectangle_presenter.h"

#include "views/circle_view.h"
#include "views/line_view.h"
#include "views/rectangle_view.h"

#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"

std::shared_ptr<ShapePresenter>	makeShapePresenter(std::shared_ptr<ShapeView> view,std::shared_ptr<Shape> shape)
{
    switch (shape -> type())
    {
	case ShapeType::Circle:
	{
	    auto v = std::dynamic_pointer_cast<CircleView>(view);
	    auto s = std::dynamic_pointer_cast<Circle>(shape);
	    auto r = std::make_shared<CirclePresenter>(v, s);
	    v -> setPresenter(r);
	    return r;
	}
	case ShapeType::Line:
	{
	    auto v = std::dynamic_pointer_cast<LineView>(view);
	    auto s = std::dynamic_pointer_cast<Line>(shape);
	    auto r = std::make_shared<LinePresenter>(v, s);
	    v -> setPresenter(r);
	    return r;
	}
	case ShapeType::Rectangle:
	{
	    auto v = std::dynamic_pointer_cast<RectangleView>(view);
	    auto s = std::dynamic_pointer_cast<Rectangle>(shape);
	    auto r = std::make_shared<RectanglePresenter>(v, s);
	    v -> setPresenter(r);
	    return r;
	}
    }
    return nullptr;
}


