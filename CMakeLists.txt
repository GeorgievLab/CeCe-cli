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
project(cece-cli
    VERSION 0.6.0
    LANGUAGES CXX
)

# ######################################################################### #

if (NOT TARGET cece)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CeCe-core/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init CeCe-core
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

    add_subdirectory(CeCe-core)

    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        set_property(DIRECTORY CeCe-core PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(DIRECTORY CeCe-core PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif ()

if (NOT TARGET cece-plugins)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CeCe-plugins/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init CeCe-plugins
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

    add_subdirectory(CeCe-plugins)

    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        set_property(DIRECTORY CeCe-plugins PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(DIRECTORY CeCe-plugins PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

# ######################################################################### #

# Options
if (CECE_RENDER)
    option(CECE_CLI_ENABLE_IMAGE_CAPTURE "Enable support for image capturing" On)
    option(CECE_CLI_ENABLE_VIDEO_CAPTURE "Enable support for video capturing" On)
    option(CECE_CLI_ENABLE_RENDER_THREAD "Enable support for rendering thread" Off)
endif ()

# ######################################################################### #

if (CECE_RENDER)
    # Init GLFW submodule
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/GLFW/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive GLFW
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(GLFW)
endif ()

if (CECE_CLI_ENABLE_IMAGE_CAPTURE OR CECE_CLI_ENABLE_VIDEO_CAPTURE)
    if (NOT CECE_RENDER)
        message(WARNING "Video/image capture requires rendering support")
        set(CECE_CLI_ENABLE_IMAGE_CAPTURE Off)
        set(CECE_CLI_ENABLE_VIDEO_CAPTURE Off)
    else ()
        message(STATUS "Video/image capture support")

        # LibPNG is required
        find_package(PNG REQUIRED)
        if (WIN32)
            include_directories(${PNG_PNG_INCLUDE_DIR})
        else ()
            include_directories(${PNG_INCLUDE_DIRS})
        endif ()
    endif ()
endif ()

if (CECE_CLI_ENABLE_RENDER_THREAD)
    if (NOT CECE_THREAD_SAFE)
        set(CECE_CLI_ENABLE_RENDER_THREAD Off)
        message(WARNING "No thread safe support, disabling CLI rendering thread")
    endif ()
endif ()

# ######################################################################### #

# Sources
set(SRCS
    src/Arguments.hpp
    src/Arguments.cpp
    src/Image.hpp
    src/Image.cpp
    src/Simulator.hpp
    src/Simulator.cpp
    src/main.cpp
)

# Configure config
configure_file(src/config.hpp.in config.hpp)

# Configure version file
configure_file(src/version.hpp.in version.hpp)

if (APPLE)
    set(SRCS ${SRCS} icons/CeCe.icns)
    set_source_files_properties(icons/CeCe.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)
    configure_file(src/version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

# Create executable
add_executable(${PROJECT_NAME} ${SRCS})

# ######################################################################### #

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS Off
    CXX_STANDARD_REQUIRED On
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_BINARY_DIR}
    PRIVATE GLFW/include
)

if (CECE_PLUGINS_DEV)
    get_target_property(DIR cece-plugins OUTPUT_DIRECTORY)

    # Directory is in build directory
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -DDIR_PLUGINS="${DIR}"
    )
endif ()

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    cece
)

if (CECE_RENDER)
    target_link_libraries(${PROJECT_NAME}
        glfw ${GLFW_LIBRARIES}
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (CECE_CLI_ENABLE_IMAGE_CAPTURE)
    target_link_libraries(${PROJECT_NAME}
        ${PNG_LIBRARIES}
    )
endif ()

# ######################################################################### #

if (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "\$ORIGIN/../lib")
elseif (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib")
endif ()

# ######################################################################### #

if (CECE_TESTS_BUILD)
    enable_testing()
    add_test(app_test ${PROJECT_NAME} --help)
endif()

# ######################################################################### #

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

install(FILES LICENSE.txt Copyright.txt README.md
    DESTINATION .
)

# ######################################################################### #

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CeCe CLI application")
set(CPACK_PACKAGE_VENDOR "Georgiev Lab")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_NAME CeCe-cli)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Jiří Fatka <fatkaj@ntis.zcu.cz>")

# ######################################################################### #

set(CPACK_STRIP_FILES "${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}")

# ######################################################################### #

include(CPack)

# ######################################################################### #
