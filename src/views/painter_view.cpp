#include "views/painter_view.h"
#include "presenters/painter_presenter.h"
#include <iostream>
#include <fstream>
#include <string>
void PainterView::setPresenter(std::shared_ptr<PainterPresenter> presenter) {
    m_presenter = presenter;
}

void PainterView::onDrag(const Point& where) {
    std::cout << "Drag at (" << where.x << "," << where.y << ")" << std::endl;
    m_presenter->startTransform(where);
}

void PainterView::onDrop() {
    std::cout << "Drop" << std::endl;
    m_presenter->endTransform();
}

void PainterView::onCursorMove(const Point& where) {
    m_presenter->move(where);
}

void PainterView::onAction(const Point& where) {
    std::cout << "Action at (" << where.x << "," << where.y << ")" << std::endl;
    m_presenter->action(where);
}

void PainterView::onDelete() {
    std::cout << "Delete" << std::endl;
    m_presenter->deleteActive();
}

void PainterView::onNew() {
    std::cout << "New" << std::endl;
    m_presenter->beginNew();
}

void PainterView::setFillColorOnActive([[maybe_unused]]Color c) {
    std::cout << "Set fill color" << std::endl;
}

void PainterView::setBorderColorOnActive([[maybe_unused]]Color c) {
    std::cout << "Set border color" << std::endl;
}

void PainterView::onSave() {
    std::cout << "Save" << std::endl;

    std::string filename;
    //TODO obtain filename to save
    std::fstream out(filename);
    m_presenter -> save(out);
}

void PainterView::onLoad() {
    std::cout << "Load" << std::endl;
    std::string filename;
    //TODO obtain filename to load
    std::fstream in(filename);
    m_presenter -> load(in);
}

void PainterView::updateView() {
    std::cout << "Updating view" << std::endl;
//    m_presenter->update();
}