cmake_minimum_required(VERSION 3.10)
project(SharedMemoryExample)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})


# add_executable(cvnotify cvnotify.cpp)
# add_executable(cvwait cvwait.cpp)
add_executable(nm nm.cpp)

target_link_libraries(nm rt pthread)

# target_link_libraries(cvnotify rt pthread)
# target_link_libraries(cvwait rt pthread)
