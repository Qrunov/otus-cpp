#pragma once
#include <memory>
//#include "../presenters/shape_presenter.h"
//#include "shape_presenter.h"

class ShapePresenter;
class ShapeView {
public:
    virtual ~ShapeView() = default;
    virtual void updateView() const = 0;
    
    void setPresenter(std::shared_ptr<ShapePresenter> presenter);
    void activate();
    void deactivate();

protected:
    bool isActive() const { return m_isActive; }
    std::shared_ptr<ShapePresenter> getPresenter() const { return m_presenter; }

private:
    bool m_isActive{false};
    std::shared_ptr<ShapePresenter> m_presenter;
};