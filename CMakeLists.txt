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
        sylar/http/http_connection.cpp
        sylar/http/http_parser.cpp
        sylar/http/http_session.cpp
        sylar/http/http_server.cpp
        sylar/http/servlet.cpp
        sylar/http/http11_parser.rl.cpp
        sylar/http/httpclient_parser.rl.cpp
        sylar/hook.cpp
        sylar/iomanager.cpp
        sylar/log.cpp
        sylar/scheduler.cpp
        sylar/socket.cpp
        sylar/socket_stream.cpp
        sylar/stream.cpp
        sylar/tcp_server.cpp
        sylar/thread.cpp
        sylar/timer.cpp
        sylar/util.cpp)

ragelmaker(sylar/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar/http)
ragelmaker(sylar/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/sylar)

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")

set(LIBS
        sylar
        dl
        yaml-cpp
        pthread)

sylar_add_executable(test1 "tests/test.cpp" sylar "${LIBS}")
sylar_add_executable(test_config "tests/test_config.cpp" sylar "${LIBS}")
sylar_add_executable(test_thread "tests/test_thread.cpp" sylar "${LIBS}")
sylar_add_executable(test_util "tests/test_util.cpp" sylar "${LIBS}")
sylar_add_executable(test_fiber "tests/test_fiber.cpp" sylar "${LIBS}")
sylar_add_executable(test_scheduler "tests/test_scheduler.cpp" sylar "${LIBS}")
sylar_add_executable(test_iomanager "tests/test_iomanager.cpp" sylar "${LIBS}")
sylar_add_executable(test_hook "tests/test_hook.cpp" sylar "${LIBS}")
sylar_add_executable(test_address "tests/test_address.cpp" sylar "${LIBS}")
sylar_add_executable(test_socket "tests/test_socket.cpp" sylar "${LIBS}")
sylar_add_executable(test_bytearray "tests/test_bytearray.cpp" sylar "${LIBS}")
sylar_add_executable(test_http "tests/test_http.cpp" sylar "${LIBS}")
sylar_add_executable(test_http_parser "tests/test_http_parser.cpp" sylar "${LIBS}")
sylar_add_executable(test_tcp_server "tests/test_tcp_server.cpp" sylar "${LIBS}")
sylar_add_executable(echo_server "examples/echo_server.cpp" sylar "${LIBS}")
sylar_add_executable(test_http_server "tests/test_http_server.cpp" sylar "${LIBS}")
sylar_add_executable(test_http_connection "tests/test_http_connection.cpp" sylar "${LIBS}")
sylar_add_executable(test_uri "tests/test_uri.cpp" sylar "${LIBS}")


#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/lib)