#pragma once
#include <memory>
#include <list>
#include "common/point.h"
#include "model/painter_model.h"
//#include "../views/painter_view.h"
#include "presenters/shape_presenter.h"
class PainterView;

class PainterPresenter {
public:
    enum class State { Select, Line, Circle, Rectangle };
    
    PainterPresenter(std::shared_ptr<PainterModel> model,
                    std::shared_ptr<PainterView> view):
    m_state(State::Select), m_model(model), m_view(view) {}
    
    void action(const Point& where);
    void deleteActive();
    void beginNew();
    void save(std::ostream&) const;
    void load(std::istream&);
    void startTransform(const Point&);
    void endTransform();
    void move(const Point&);
    void update();
    
    void setState(State state) { m_state = state; }
    State getState() const { return m_state; }

private:
    void activate(std::shared_ptr<Shape> selected);
    
    State m_state{State::Select};
    std::shared_ptr<ShapePresenter> m_active;
    std::shared_ptr<PainterModel> m_model;
    std::shared_ptr<PainterView> m_view;
};