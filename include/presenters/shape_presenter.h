#pragma once
#include <memory>
#include "common/point.h"
#include "views/shape_view.h"
#include "shapes/shape.h"

class ShapePresenter {
public:
    ShapePresenter(std::shared_ptr<ShapeView> v, std::shared_ptr<Shape> m);
    virtual ~ShapePresenter();
    
    void update();
    void activate();
    bool isInside(Point where);
    
    virtual void startTransform(const Point& start);
    virtual void endTransform();
    virtual void doTransform(const Point& p) = 0;
    
    bool isOnTransform() const { return m_onTransform; }
    std::shared_ptr<Shape> getModel() const { return m_model; }

protected:
    Point m_startTransformPoint;
    std::shared_ptr<ShapeView> m_view;
    std::shared_ptr<Shape> m_model;
private:
    bool m_onTransform{false};
};