

# Download and unpack googletest at configure time
configure_file(${CMAKE_SOURCE_DIR}/cmake/GoogleTest-CMakeLists.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)


if(VCPKG_TOOLCHAIN) #or whatever variable we find vcpkg sets
find_package(protobuf REQUIRED)
set(Protobuf_IMPORT_DIRS  ${Protobuf_INCLUDE_DIRS})
else()
find_package(Protobuf REQUIRED)
set(protobuf_INCLUDE_DIRS Protobuf_INCLUDE_DIRS)
endif()
                 
find_package(Threads REQUIRED)

                 
set(TBM_TEST_SRCS src/test_protofileloader.cpp
                src/test_protomessageserializer.cpp
                src/test_protomessagehttpclient.cpp
                src/httpserver.cpp
                src/prototestutils.cpp
                )

			
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS  ${CMAKE_CURRENT_SOURCE_DIR}/data/addressbook.proto)

include_directories(proto_cpp/ ${EASYLOGGING_INCLUDE})

set(EXECUTABLE_NAME tbm_test)

add_executable(${EXECUTABLE_NAME} ${TBM_TEST_SRCS} ${PROTO_SRCS} ${PROTO_HDRS} ${EASYLOGGING_SRCS})

if (WIN32)
# copy the .dll file to the same folder as the executable
add_custom_command(
    TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:TBM::tbm>
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC "TBM_TEST_LINUX")
endif()

#target_compile_features(json_utils_test PRIVATE cxx_auto_type)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE gtest_main TBM::tbm )

