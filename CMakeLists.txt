#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
#
# https://axmol.dev/
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.20)

set(APP_NAME bento-boy)

project(${APP_NAME})

if (${DEBUG})
    add_definitions(-DDEBUG=1)
else ()
    add_definitions(-DRELEASE=1)
endif ()
add_definitions(-DGENERIC_APP_NAME="${APP_NAME}")

if(XCODE)
    set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
endif()

set(_is_axmol_embed FALSE)
set(_AX_USE_PREBUILT FALSE)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

# NOTE: The order of the cmake module "include(AXGame...)" statements matters
include(AXGameEngineSetup)

# record sources, headers, resources...
set(GAME_HEADER)
set(GAME_SOURCE)

# add cross-platforms source files and header files
list(APPEND GAME_HEADER
        src/AppDelegate.h
)
list(APPEND GAME_SOURCE
        src/AppDelegate.cpp
)
# generic modules
add_subdirectory(src/generic)
# current game modules
add_subdirectory(src/databaseModule)
add_subdirectory(src/gameplayModule)
add_subdirectory(src/interfaceModule)
add_subdirectory(src/profileModule)
add_subdirectory(src/sceneModule)

set(GAME_INC_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(content_folder
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)

if(APPLE)
    ax_mark_multi_resources(common_content_files RES_TO "Resources" FOLDERS ${content_folder})
elseif(WINDOWS)
    ax_mark_multi_resources(common_content_files RES_TO "Content" FOLDERS ${content_folder})
endif()

include(AXGameSourceSetup)

# mark app complie info and libs info
set(APP_SOURCES
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

include(AXGameTargetSetup)

# mark app resources, resource will be copy auto after mark
ax_setup_app_config(${APP_NAME})

# Add any libraries you need to link to the project after this point
target_link_libraries(${APP_NAME} ${GENERIC_LIB_NAME})
target_include_directories(${APP_NAME} PRIVATE src)
target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
target_compile_options(${APP_NAME} PRIVATE -Wall)

# Default Platform-specific setup
include(AXGamePlatformSetup)

# Make sure the AXGameFinalSetup is included at the end of this file
include(AXGameFinalSetup)
