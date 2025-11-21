cmake_minimum_required(VERSION 3.5)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIR} .)

option(WITH_GOOGLE_TEST "Whether to build google test" ON)


add_executable(join_server join_server.cpp table.cpp controller.cpp command.cpp)

set_target_properties(join_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(join_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )


if(WITH_GOOGLE_TEST)
    # Ищем Google Test
    find_package(GTest REQUIRED)

    # Создаём тестовый исполняемый файл
    add_executable(
      test_join
      table.cpp
      test.cpp)

    set_target_properties(test_join PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    # Подключаем gtest
    target_link_libraries(
        test_join
    	GTest::GTest GTest::Main
    )

endif()

install(TARGETS test_join RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(test_join test_join)
endif()


