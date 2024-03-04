cmake_minimum_required(VERSION 3.7)
project(rayModel)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp 
    ${CMAKE_SOURCE_DIR}/src/viewPortCamera.cpp 
    ${CMAKE_SOURCE_DIR}/src/ShaderBuilder.cpp
    ${CMAKE_SOURCE_DIR}/src/SDFTree.cpp 
    ${CMAKE_SOURCE_DIR}/src/UI/ToolBar.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/TreeView.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/NodeProperties.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/Panel.cpp
    ) 

add_subdirectory(${CMAKE_SOURCE_DIR}/bgfx.cmake) 
add_executable(rayModel ${SOURCE_FILES} )

target_link_libraries(rayModel bgfx example-common)