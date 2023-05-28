cmake_minimum_required(VERSION 3.16)
project(sylar)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)


set(LIB_SRC
        sylar/address.cpp
        sylar/bytearray.cpp
        sylar/config.cpp
        sylar/fd_manager.cpp
        sylar/fiber.cpp
        sylar/http/http.cpp
        sylar/http/http_parser.cpp
        sylar/http/http11_parser.rl.cpp
        sylar/http/httpclient_parser.rl.cpp
        sylar/hook.cpp
        sylar/iomanager.cpp
        sylar/log.cpp
        sylar/scheduler.cpp
        sylar/socket.cpp
        sylar/thread.cpp
        sylar/timer.cpp
        sylar/util.cpp)

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIB_LIB
        sylar
        dl
        yaml-cpp
        pthread)

add_executable(test tests/test.cpp )
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test)
target_link_libraries(test ${LIB_LIB})

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIB_LIB})

add_executable(test_util tests/test_util.cpp)
add_dependencies(test_util sylar)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cpp)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cpp)
add_dependencies(test_scheduler sylar)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler ${LIB_LIB})

add_executable(test_iomanager tests/test_iomanager.cpp)
add_dependencies(test_iomanager sylar)
force_redefine_file_macro_for_sources(test_iomanager)
target_link_libraries(test_iomanager ${LIB_LIB})

add_executable(test_hook tests/test_hook.cpp)
add_dependencies(test_hook sylar)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook ${LIB_LIB})

add_executable(test_address tests/test_address.cpp)
add_dependencies(test_address sylar)
force_redefine_file_macro_for_sources(test_address)
target_link_libraries(test_address ${LIB_LIB})

add_executable(test_socket tests/test_socket.cpp)
add_dependencies(test_socket sylar)
force_redefine_file_macro_for_sources(test_socket)
target_link_libraries(test_socket ${LIB_LIB})

add_executable(test_bytearray tests/test_bytearray.cpp)
add_dependencies(test_bytearray sylar)
force_redefine_file_macro_for_sources(test_bytearray)
target_link_libraries(test_bytearray ${LIB_LIB})

add_executable(test_http tests/test_http.cpp)
add_dependencies(test_http sylar)
force_redefine_file_macro_for_sources(test_http)
target_link_libraries(test_http ${LIB_LIB})

add_executable(test_http_parser tests/test_http_parser.cpp)
add_dependencies(test_http_parser sylar)
force_redefine_file_macro_for_sources(test_http_parser)
target_link_libraries(test_http_parser ${LIB_LIB})


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/lib)