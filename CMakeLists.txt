cmake_minimum_required(VERSION 3.15)

project(software-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB RENDER_HEADERS
        *.h
        core/*.h
        render/*.h
)

file(GLOB RENDER_SOURSES
        *.cpp
        core/*.cpp
        render/*.cpp
)

add_executable(${PROJECT_NAME} ${RENDER_HEADERS} ${RENDER_SOURSES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
