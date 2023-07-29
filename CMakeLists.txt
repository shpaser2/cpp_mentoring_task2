cmake_minimum_required(VERSION 3.16)

project(myProg CXX)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    main.cpp
    rw.cpp
)

set(HEADERS
    rw.h
)

find_package (Threads REQUIRED)
find_package(Boost REQUIRED)

add_executable(myProg ${SOURCES} ${HEADERS})
target_link_libraries (myProg Threads::Threads Boost::boost)
 