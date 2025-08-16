#include "../include/model/painter_model.h"
#include "../include/views/painter_view.h"
#include "../include/presenters/painter_presenter.h"
#include <iostream>
#include <memory>

using namespace std;

int main() {
    // Инициализация компонентов MVP
    auto model = make_shared<PainterModel>();
    auto view = make_shared<PainterView>();
    auto presenter = make_shared<PainterPresenter>(model, view);
    
    view->setPresenter(presenter);

    cout << "Painter Application Started" << endl;
    cout << "--------------------------" << endl;

    // Тестовые взаимодействия
    Point testPoint{100, 100};
    
    // Создаем линию
    presenter->setState(PainterPresenter::State::Line);
    view->onAction(testPoint);
    
    // Создаем прямоугольник
    presenter->setState(PainterPresenter::State::Rectangle);
    view->onAction({150, 150});
    
    // Создаем круг
    presenter->setState(PainterPresenter::State::Circle);
    view->onAction({200, 200});
    
    // Переключаемся в режим выбора
    presenter->setState(PainterPresenter::State::Select);
    
    // Тестируем выбор объекта
    view->onAction(testPoint);
    
    // Тестируем перемещение
    view->onDrag(testPoint);
    view->onCursorMove({120, 120});
    view->onDrop();
    
    // Тестируем сохранение/загрузку
//    cout << "\nSaving model..." << endl;
//    view->onSave();
    
//    cout << "\nLoading model..." << endl;
//    view->onLoad();
    
    // Тестируем удаление
    view->onDelete();
    
    // Тестируем новое полотно
    view->onNew();

    cout << "\nPainter Application Finished" << endl;
    return 0;
}