find_package(Qt5 REQUIRED COMPONENTS Gui Qml QuickControls2 Sql Quick)

set(CMAKE_AUTOMOC ON)


qt5_add_resources(TAMBOURINEMAN_RS_SRCS resources.qrc)

file(GLOB_RECURSE HEADERS *.h *.hpp )

set(EXECUTABLE_NAME tambourineman)

set (TAMBOURINEMAN_SRCS main.cpp protofileloadermodel.cpp)

include_directories("${CMAKE_SOURCE_DIR}/lib/include" ${Protobuf_INCLUDE_DIRS})


add_executable(${EXECUTABLE_NAME} ${TAMBOURINEMAN_SRCS} ${TAMBOURINEMAN_RS_SRCS})

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "TAMBOURINEMAN_DEBUG=$<CONFIG:Debug>")

add_custom_target(Headers SOURCES ${HEADERS})

target_link_libraries(${EXECUTABLE_NAME} Qt5::Quick Qt5::Sql Qt5::QuickControls2
    Qt5::Qml Qt5::Gui)
target_link_libraries(${EXECUTABLE_NAME} TBM::tbm)

add_executable("${EXECUTABLE_NAME}::source" ALIAS "${EXECUTABLE_NAME}")

if(UNIX AND NOT APPLE)
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "TBM_QT_LINUX")
endif()


install(TARGETS "${EXECUTABLE_NAME}" EXPORT "${EXECUTABLE_NAME}-targets"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT Runtime
)
