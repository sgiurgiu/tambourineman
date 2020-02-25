find_package(Qt5 REQUIRED COMPONENTS Sql Widgets)

set(CMAKE_AUTOMOC ON)

qt5_add_resources(TAMBOURINEMAN_RS_SRCS resources.qrc)

file(GLOB_RECURSE HEADERS *.h *.hpp )

set(EXECUTABLE_NAME tambourineman)

set (TAMBOURINEMAN_SRCS main.cpp
    tbmmainwindow.cpp
    protofileloadermodel.cpp
    protomessageoperationswidget.cpp)

include_directories("${CMAKE_SOURCE_DIR}/lib/include" ${Protobuf_INCLUDE_DIRS})


add_executable(${EXECUTABLE_NAME} ${TAMBOURINEMAN_SRCS} ${TAMBOURINEMAN_RS_SRCS})

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "$<$<CONFIG:Debug>:TAMBOURINEMAN_DEBUG>")

add_custom_target(Headers SOURCES ${HEADERS})

target_link_libraries(${EXECUTABLE_NAME} Qt5::Sql Qt5::Widgets)
target_link_libraries(${EXECUTABLE_NAME} TBM::tbm)


if (WIN32)
# copy the .dll file to the same folder as the executable
add_custom_command(
    TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:TBM::tbm>
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
endif()

add_executable("${EXECUTABLE_NAME}::source" ALIAS "${EXECUTABLE_NAME}")

if(UNIX AND NOT APPLE)
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "TBM_QT_LINUX")
endif()
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "TBM_QT_VERSION=\"${CMAKE_PROJECT_VERSION}\"")


install(TARGETS "${EXECUTABLE_NAME}" EXPORT "${EXECUTABLE_NAME}-targets"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT Runtime
)
