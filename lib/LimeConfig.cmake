##############################################################################
#                                                                            #
# This file is part of lime, a lightweight C++ segmentation library          #
#                                                                            #
# Copyright (C) 2012 Alexandru Duliu                                         #
#                                                                            #
# lime is free software; you can redistribute it and/or                      #
# modify it under the terms of the GNU Lesser General Public                 #
# License as published by the Free Software Foundation; either               #
# version 3 of the License, or (at your option) any later version.           #
#                                                                            #
# lime is distributed in the hope that it will be useful, but WITHOUT ANY    #
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  #
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the #
# GNU General Public License for more details.                               #
#                                                                            #
# You should have received a copy of the GNU Lesser General Public           #
# License along with lime. If not, see <http://www.gnu.org/licenses/>.       #
#                                                                            #
##############################################################################

# Config file for the lime library
# It defines the following variables
#
# Lime_INCLUDE_DIR - include directory for lime headers
# Lime_INCLUDE_DIRS - all include directories lime needs
# Lime_LIBRARY - library

# version
set(Lime_MAJOR_VERSION 0)
set(Lime_MINOR_VERSION 1)
set(Lime_PATCH_VERSION 0)
set(Lime_VERSION ${Lime_MAJOR_VERSION}.${Lime_MINOR_VERSION}.${Lime_PATCH_VERSION})

# set path
set( Lime_DIR ${CMAKE_CURRENT_LIST_DIR})
set( ENV{Lime_DIR} ${Lime_DIR} )

# add module paths
list( APPEND CMAKE_MODULE_PATH ${Lime_DIR}/cmake ${CMAKE_INSTALL_PREFIX}/share $ENV{HOME}/.local/share )

# find Eigen3
if( WIN32 )
    set( EIGEN3_INCLUDE_DIR $ENV{Eigen3_DIR} )
endif()
find_package( Eigen3 REQUIRED )

# set the include dir
set( Lime_INCLUDE_DIR "${Lime_DIR}/include")

# set target names
set( Lime_TARGET lime )

# set compile definitions
set( Lime_COMPILE_DEFINITIONS LIME CACHE INTERNAL "all compile definitions lime needs"  )

# if this is 32-bit, disable alignment
if( NOT CMAKE_SIZEOF_VOID_P MATCHES "8")
    list( APPEND Lime_COMPILE_DEFINITIONS EIGEN_DONT_ALIGN)
endif()

# set linker flags
if( WIN32 )
	list( APPEND Lime_LINK_FLAGS " /MANIFEST:NO" )
endif()


# set library paths
set( Lime_LIBRARY ${Lime_TARGET} CACHE INTERNAL "the lime lib" )

# set include directories
set( Lime_INCLUDE_DIRS
    ${Lime_INCLUDE_DIR}
    ${Lime_DIR}/extern/cimg
    ${EIGEN3_INCLUDE_DIR} CACHE INTERNAL "all include directories lime needs" )

# link libraries
set( Lime_LINK_LIBRARIES
    -lm
    -lc CACHE INTERNAL "all libs lime needs" )


# enable C++11 support
if( NOT WIN32 )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
    else( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Qunused-arguments")
    endif()
endif()
