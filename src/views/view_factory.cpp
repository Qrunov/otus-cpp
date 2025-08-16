#include <memory>
#include "views/shape_view.h"
#include "views/line_view.h"
#include "views/circle_view.h"
#include "views/rectangle_view.h"


std::shared_ptr<ShapeView>	makeShapeView(ShapeType	type)
{
    switch (type)
    {
	case ShapeType::Circle:
	    return std::make_shared<CircleView>();

	case ShapeType::Line:
	    return std::make_shared<LineView>();

	case ShapeType::Rectangle:
	    return std::make_shared<RectangleView>();
    }
    return nullptr;
}

