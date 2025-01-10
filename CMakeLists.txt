cmake_minimum_required(VERSION 3.16)

project(TinyRayTracing VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
qt_add_executable(TinyRayTracing ${SOURCES} ${HEADERS})

include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

target_link_libraries(TinyRayTracing PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
