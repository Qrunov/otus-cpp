cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(painter VERSION ${PROJECT_VERSION})
# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем подкаталоги
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shapes
    ${CMAKE_CURRENT_SOURCE_DIR}/include/views
    ${CMAKE_CURRENT_SOURCE_DIR}/include/presenters
    ${CMAKE_CURRENT_SOURCE_DIR}/include/model
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common
)


# Собираем исполняемый файл
add_executable(painter
    main.cpp
    
    # Модель
    src/model/painter_model.cpp
    
    # Фигуры
    src/shapes/shape.cpp
    src/shapes/line.cpp
    src/shapes/circle.cpp
    src/shapes/rectangle.cpp
    src/shapes/shape_factory.cpp
    
    # Представления
    src/views/shape_view.cpp
    src/views/line_view.cpp
    src/views/circle_view.cpp
    src/views/rectangle_view.cpp
    src/views/painter_view.cpp
    src/views/view_factory.cpp

    # Презентеры
    src/presenters/shape_presenter.cpp
    src/presenters/line_presenter.cpp
    src/presenters/circle_presenter.cpp
    src/presenters/rectangle_presenter.cpp
    src/presenters/painter_presenter.cpp
    src/presenters/shape_presenter.cpp
    src/presenters/presenter_factory.cpp
)
target_compile_options(painter PRIVATE
    -Wall -Wextra -pedantic -Werror
)
install(TARGETS painter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
