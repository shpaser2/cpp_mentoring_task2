cmake_minimum_required(VERSION 3.16)

project(myProg CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER $ENV{CXX})

set(SOURCES
    main.cpp
    rw.cpp
)

set(HEADERS
    rw.h
)

find_package (Threads REQUIRED)

add_executable(myProg ${SOURCES} ${HEADERS})
target_link_libraries (myProg Threads::Threads)
