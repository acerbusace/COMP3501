cmake_minimum_required(VERSION 2.6)

# Name of project
project(Extermination)

# Specify project files: header files and source files
set(HDRS
    bomb.h camera.h game.h laser.h misc.h missile.h orb.h player.h resource.h resource_manager.h scene_graph.h scene_node.h tower.h tower_control.h tank.h tank_control.h
)

set(SRCS
    bomb.cpp camera.cpp game.cpp laser.cpp misc.cpp main.cpp missile.cpp orb.cpp player.cpp resource.cpp resource_manager.cpp scene_graph.cpp scene_node.cpp tower.cpp tower_control.cpp tank.cpp tank_control.cpp material_vp.glsl material_fp.glsl shiny_blue_fp.glsl  shiny_blue_vp.glsl shiny_texture_fp.glsl shiny_texture_vp.glsl explosion_fp.glsl explosion_gp.glsl explosion_vp.glsl spline_fp.glsl spline_gp.glsl spline_vp.glsl
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(Extermination ${HDRS} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Extermination ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(Extermination ${GLEW_LIBRARY})
target_link_libraries(Extermination ${GLFW_LIBRARY})
target_link_libraries(Extermination ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)

    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(Extermination PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
