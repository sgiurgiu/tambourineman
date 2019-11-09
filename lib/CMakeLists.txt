project (tbm
     VERSION 0.0.1
     LANGUAGES C CXX
     DESCRIPTION "LibTambourineMan - Your API tester"
    )

find_package(Protobuf REQUIRED)
find_package (Threads REQUIRED)

file(GLOB_RECURSE LIB_HEADERS *.h *.hpp )

set(LIB_NAME tbm)
set (TBM_SRCS src/protofileloader.cpp
        src/protofile.cpp)

include_directories("${CMAKE_SOURCE_DIR}/lib/include" ${Protobuf_INCLUDE_DIRS}
    ${EASYLOGGING_INCLUDE})

add_library(${LIB_NAME} SHARED ${TBM_SRCS} ${EASYLOGGING_SRCS})
add_library(TBM::${LIB_NAME} ALIAS ${LIB_NAME})

target_compile_definitions(${LIB_NAME} PUBLIC "LIBTBM_DEBUG=$<CONFIG:Debug>")

target_include_directories(${LIB_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_target(LibHeaders SOURCES ${LIB_HEADERS})

target_link_libraries(${LIB_NAME} PUBLIC ${Protobuf_LIBRARIES})
target_link_libraries(${LIB_NAME} PRIVATE Threads::Threads)

## this was copied and mangled until it became a spagetti mess
## from https://github.com/forexample/package-example/blob/master/Foo/CMakeLists.txt
## TODO: revisit this at some point in the next decade

include(GNUInstallDirs)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${LIB_NAME}Targets")


configure_package_config_file(
    "cmake/${LIB_NAME}Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    TARGETS ${LIB_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    FILE   "${TARGETS_EXPORT_NAME}.cmake"
    NAMESPACE "TBM::"
    DESTINATION "${config_install_dir}"
)

export(EXPORT "${TARGETS_EXPORT_NAME}"
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_EXPORT_NAME}.cmake"
    NAMESPACE TBM::)

#Register package in user's package registry
export(PACKAGE TBM)
