cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(server mysql.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Установить флаги компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -I/usr/include/cppconn")
# Добавить библиотеку MySQL Connector/C++
find_library(MYSQL_CONNECTOR_CPP_LIBRARY mysqlcppconn)
target_link_libraries(${PROJECT_NAME} ${MYSQL_CONNECTOR_CPP_LIBRARY})

include(CPack)