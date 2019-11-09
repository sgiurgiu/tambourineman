cmake_minimum_required(VERSION 3.10)

project (tambourineman
     VERSION 0.0.1
     LANGUAGES C CXX
     DESCRIPTION "TambourineMan - Your API tester"
    )


OPTION(ENABLE_TESTING "Enable generation and compilation of tests" ON)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
endif()

if(CMAKE_CXX_STANDARD)
    set(conan_std cppstd=${CMAKE_CXX_STANDARD})
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
else()
    add_compile_options(/permissive-)
    # We want to pass this flag to link.exe. In CMake 3.13, we can use add_link_options
    # /IGNORE:4221 # LNK4221 warns for empty cpp files. We use such files.
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(EASYLOGGING_DIR ${CMAKE_SOURCE_DIR}/external/easyloggingpp/)
set(EASYLOGGING_INCLUDE ${EASYLOGGING_DIR}/src)
set(EASYLOGGING_SRCS ${EASYLOGGING_DIR}/src/easylogging++.cc)


add_subdirectory(lib)
add_subdirectory(qt)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/installation/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/"
)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installation/${PROJECT_NAME}Config.cmake"
    COMPONENT Development
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)


# Write and install a version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/installation/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY "ExactVersion"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installation/${PROJECT_NAME}ConfigVersion.cmake"
    COMPONENT Development
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/"
)


set(CPACK_PACKAGE_NAME "${PROJECT_NAME}"                                )
set(CPACK_PACKAGE_VENDOR "Sergiu Giurgiu"                         )
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tambourine Man"    )
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md" )


include(CPack)

if(ENABLE_TESTING)
    add_subdirectory(test)
endif(ENABLE_TESTING)
