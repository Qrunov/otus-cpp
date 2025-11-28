cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(fashio_mlp VERSION ${PROJECT_VERSION})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)
add_executable(fashio_mlp
    src/fashio_mlp.cpp
    src/fashio_mlp.cpp
    src/helpers.cpp 
    src/mlp_classifier.cpp
)

target_include_directories(fashio_mlp PUBLIC
    src
)


target_link_libraries(fashio_mlp PUBLIC
    Eigen3::Eigen
)


install(TARGETS fashio_mlp RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
install(FILES ${CMAKE_SOURCE_DIR}/data/w1.txt  DESTINATION share/ml)
install(FILES ${CMAKE_SOURCE_DIR}/data/w2.txt  DESTINATION share/ml)
install(FILES ${CMAKE_SOURCE_DIR}/data/test.csv DESTINATION share/ml)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
