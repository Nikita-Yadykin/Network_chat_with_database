cmake_minimum_required(VERSION 3.22)
project(Server)

set (MYSQLCONNECTORCPP_ROOT_DIR "C:/Program Files (x86)/MySQL/MySQL Connector C++ 8.0")

### Include paths ###

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${UTILITIES_DIR})
include_directories (${MYSQLCONNECTORCPP_ROOT_DIR})
include_directories (${MYSQLCONNECTORCPP_ROOT_DIR}\\include\\jdbc)
include_directories (/usr/local/include)

link_directories (/usr/local/lib)
link_directories (${MYSQLCONNECTORCPP_ROOT_DIR}/driver)
link_directories (${MYSQLCONNECTORCPP_ROOT_DIR}/lib/vs14)
link_directories (/usr/lib/mysql/)
link_directories (/usr/lib/)
link_directories (/usr/local/mysql/lib/)

add_executable(Server main.cpp sha1.h sha1.cpp User.h User.cpp Message.h Message.cpp ConsoleChat.h ConsoleChat.cpp NetWorkStream.h NetWorkStream.cpp)
target_link_libraries(Server mysqlcppconn)