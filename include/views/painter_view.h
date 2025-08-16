#pragma once
#include <memory>
#include "../common/point.h"
#include "../common/enums.h"
#include "../presenters/painter_presenter.h"

class PainterView {
public:
    void setPresenter(std::shared_ptr<PainterPresenter> presenter);
    
    void onDrag(const Point& where);
    void onDrop();
    void onCursorMove(const Point& where);
    void onAction(const Point& where);
    void onDelete();
    void onNew();
    void setFillColorOnActive(Color c);
    void setBorderColorOnActive(Color c);
    void onSave();
    void onLoad();
    void updateView();

private:
    std::shared_ptr<PainterPresenter> m_presenter;
};