cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(tuple VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(tuple main.cpp)

set_target_properties(tuple PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

    target_compile_options(tuple PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

install(TARGETS tuple RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

