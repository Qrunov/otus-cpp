cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(mapreducer VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build google test" ON)

configure_file(version.h.in version.h)

add_executable(mapper mapper.cpp)
add_executable(reducer1 reducer1.cpp)
add_executable(reducer2 reducer2.cpp disp.cpp)


set_target_properties(mapper PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(reducer1 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(reducer2 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)


target_include_directories(reducer2
    PRIVATE "${CMAKE_BINARY_DIR}"
)


if(WITH_GOOGLE_TEST)
    # Ищем Google Test
    find_package(GTest REQUIRED)

    # Создаём тестовый исполняемый файл
    add_executable(
      test_disp
      test.cpp
      disp.cpp)

    set_target_properties(test_disp PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    # Подключаем gtest
    target_link_libraries(
        test_disp
    	GTest::GTest GTest::Main
    )
    target_compile_options(test_disp PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()
target_compile_options(mapper PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(reducer1 PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(reducer2 PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS mapper RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
install(TARGETS reducer1 RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
install(TARGETS reducer2 RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(test_disp test_disp)
endif()


