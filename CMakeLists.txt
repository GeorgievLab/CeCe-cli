# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

cmake_minimum_required(VERSION 3.1)

# ######################################################################### #

# Project
project(cece-cli VERSION 0.6.0 LANGUAGES CXX)

# C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_POSITION_INDEPENDENT_CODE On)

include_directories(
    CeCe
    ${CMAKE_CURRENT_BINARY_DIR}/CeCe
)

# ######################################################################### #

if (NOT TARGET cece)
    add_subdirectory(CeCe)
endif ()

if (NOT TARGET cece-plugins)
    add_subdirectory(plugins)
endif()

# ######################################################################### #

# Options
if (CECE_RENDER_ENABLE)
    option(CECE_ENABLE_IMAGE_CAPTURE "Enable support for image capturing" On)
    option(CECE_ENABLE_VIDEO_CAPTURE "Enable support for video capturing" On)
    option(CECE_ENABLE_RENDER_THREAD "Enable support for rendering thread" Off)
endif ()

# ######################################################################### #

# Set some definitions
add_definitions(
    -DAPP_NAME="CeCe"
)

if (DEV_PLUGINS)
    # Directory is in build directory
    add_definitions(
        -DDIR_PLUGINS="${CMAKE_BINARY_DIR}/plugins"
    )
endif ()

if (CECE_RENDER_ENABLE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
endif ()

if (CECE_ENABLE_IMAGE_CAPTURE OR CECE_ENABLE_VIDEO_CAPTURE)
    if (NOT CECE_RENDER_ENABLE)
        message(WARNING "Video/image capture requires rendering support")
        set(CECE_ENABLE_IMAGE_CAPTURE Off)
        set(CECE_ENABLE_VIDEO_CAPTURE Off)
    else ()
        message(STATUS "Video/image capture support")

        # LibPNG is required
        find_package(PNG REQUIRED)
        if (WIN32)
            include_directories(${PNG_PNG_INCLUDE_DIR})
        else ()
            include_directories(${PNG_INCLUDE_DIRS})
        endif ()

        if (CECE_ENABLE_IMAGE_CAPTURE)
            add_definitions(-DCECE_ENABLE_IMAGE_CAPTURE=1)
        endif ()

        if (CECE_ENABLE_VIDEO_CAPTURE)
            add_definitions(-DCECE_ENABLE_VIDEO_CAPTURE=1)
        endif ()
    endif ()
endif ()

if (CECE_ENABLE_RENDER_THREAD)
    if (CECE_THREAD_SAFE)
        add_definitions(-DCECE_CLI_ENABLE_RENDER_THREAD)
    else ()
        message(WARNING "No thread safe support, disabling CLI rendering thread")
    endif ()
endif ()

# ######################################################################### #

# Sources
set(SRCS
    Arguments.hpp
    Arguments.cpp
    Image.hpp
    Image.cpp
    Simulator.hpp
    Simulator.cpp
    main.cpp
)

# Configure config
configure_file(config.hpp.in config.hpp @ONLY)

# Configure version file
configure_file(version.hpp.in version.hpp @ONLY)
include_directories(${PROJECT_BINARY_DIR})

if (UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN/../lib")
endif ()

if (APPLE)
    set(SRCS ${SRCS} icons/${APP_NAME}.icns)
    set_source_files_properties(icons/${APP_NAME}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)
    configure_file(version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

# Create executable
add_executable(${PROJECT_NAME}
    ${SRCS}
)

# ######################################################################### #

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../plugins")
endif ()

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    cece
)

if (CECE_RENDER_ENABLE)
    target_link_libraries(${PROJECT_NAME}
        glfw ${GLFW_LIBRARIES}
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (CECE_ENABLE_IMAGE_CAPTURE)
    target_link_libraries(${PROJECT_NAME}
        ${PNG_LIBRARIES}
    )
endif ()

# ######################################################################### #
