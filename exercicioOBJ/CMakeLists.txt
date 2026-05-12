cmake_minimum_required(VERSION 3.10)
project(CG_Exercise)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# GLM – header only, look in common paths
find_path(GLM_INCLUDE_DIR glm/glm.hpp
    PATHS /usr/include /usr/local/include /usr/include/glm
)

add_executable(cg_exercise main.cpp)

target_include_directories(cg_exercise PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
)

target_link_libraries(cg_exercise
    OpenGL::GL
    glfw
    GLEW::GLEW
)

# On Linux also need dl and m
if(UNIX AND NOT APPLE)
    target_link_libraries(cg_exercise dl m)
endif()
