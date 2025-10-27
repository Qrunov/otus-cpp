cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build google test" ON)

configure_file(version.h.in version.h)

add_executable(bulk_async main.cc)

target_link_libraries(
    bulk_async
    pthread
    async
)

add_library (async SHARED consoleDestination.cc fileDestination.cc wqDestination.cc observer.cc collector.cc source.cc parser.cc async.cc)

set_target_properties(bulk_async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(bulk_async
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_GOOGLE_TEST)
    # Ищем Google Test
    find_package(GTest REQUIRED)

    # Создаём тестовый исполняемый файл
    add_executable(
      test_bulk
      test.cc)

    set_target_properties(test_bulk PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    # Подключаем gtest
    target_link_libraries(
        test_bulk
	async
    	GTest::GTest GTest::Main
    )
endif()

if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_bulk PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_bulk PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()
install (TARGETS async LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

install(TARGETS bulk_async RUNTIME DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(test_bulk test_bulk)
endif()


