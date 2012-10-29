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


cmake_minimum_required(VERSION 2.8)
project(Lime CXX)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

# configure the project
include( LimeConfig.cmake )

# add the include files
list( APPEND Lime_INC
    include/lime/Segmentation.hpp
    include/lime/util.hpp )
list( APPEND Lime_SRC
    src/Segmentation.cpp )

# external dependencies of lime
list( APPEND Lime_EXTERN_INC extern/cimg/CImg.h )
list( APPEND Lime_EXTERN_SRC )

# set include directories
include_directories( ${Lime_INCLUDE_DIRS} )

# add library
add_library( ${Lime_TARGET} ${Lime_INC} ${Lime_SRC} ${Lime_EXTERN_INC} ${Lime_EXTERN_SRC} )
target_link_libraries( ${Lime_TARGET} ${Lime_LINK_LIBRARIES} )

# configure target
set_target_properties(${Lime_TARGET}
    PROPERTIES
        PUBLIC_HEADER "${Lime_INC}"
        COMPILE_DEFINITIONS "${Lime_COMPILE_DEFINITIONS}"
        COMPILE_FLAGS "${Lime_COMPILE_FLAGS}"
        LINK_FLAGS "${Lime_LINK_FLAGS}" )

export( TARGETS ${Lime_TARGET} FILE "${CMAKE_BINARY_DIR}/Lime_Depends.cmake")

# install the lib's binary and headers
install(TARGETS ${Lime_TARGET}
        EXPORT Lime
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
        PUBLIC_HEADER DESTINATION "include/lime" )

# install targets
install(EXPORT Lime DESTINATION share )

# install module finder
install(FILES "${CMAKE_CURRENT_LIST_DIR}/FindLime.cmake" DESTINATION share )

# install extern headers
install(FILES ${Lime_EXTERN_INC} DESTINATION "include" )


# add test app
add_executable( test test/test.cpp )
target_link_libraries( test ${Lime_TARGET} )

