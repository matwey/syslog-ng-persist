cmake_minimum_required (VERSION 2.6)
project(syslog-ng-persist C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSLOG REQUIRED syslog-ng)

set(CMAKE_C_FLAGS "-std=c99 -Wall")

include_directories ("${PROJECT_SOURCE_DIR}/include" ${SYSLOG_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(syslog-ng-persist ${SOURCES})
target_link_libraries(syslog-ng-persist ${SYSLOG_LIBRARIES})
install(TARGETS syslog-ng-persist DESTINATION bin)
