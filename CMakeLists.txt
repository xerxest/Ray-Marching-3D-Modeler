cmake_minimum_required(VERSION 3.7)
project(rayModel)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Main.cpp 
    ${CMAKE_SOURCE_DIR}/src/ViewPortCamera.cpp 
    ${CMAKE_SOURCE_DIR}/src/ShaderBuilder.cpp
    ${CMAKE_SOURCE_DIR}/src/SDFTree.cpp 
    ${CMAKE_SOURCE_DIR}/src/UI/ToolBar.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/TreeView.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/NodeProperties.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/Panel.cpp
    ${CMAKE_SOURCE_DIR}/src/UI/Help.cpp
    ) 

add_subdirectory(${CMAKE_SOURCE_DIR}/bgfx.cmake) 
add_executable(rayModel ${SOURCE_FILES} )
file(COPY ${CMAKE_SOURCE_DIR}/src/UI/Static DESTINATION ${CMAKE_BINARY_DIR}/)


target_link_libraries(rayModel bgfx example-common)