##############################################################################
#                                                                            #
# This file is part of LimeS, a C++ UI image segmentation                    #
#                                                                            #
# Copyright (C) 2012 Alexandru Duliu                                         #
#                                                                            #
# LimeS is free software; you can redistribute it and/or                     #
# modify it under the terms of the GNU  General Public License               #
# as published by the Free Software Foundation; either version 3             #
# of the License, or (at your option) any later version.                     #
#                                                                            #
# LimeS is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of             #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
# GNU General Public License for more details.                               #
#                                                                            #
# You should have received a copy of the GNU General Public License          #
# along with LimeS. If not, see <http://www.gnu.org/licenses/>.              #
#                                                                            #
##############################################################################


cmake_minimum_required(VERSION 2.8)
project( LimeS CXX )

# add module paths
list( APPEND CMAKE_MODULE_PATH
    ${Lime_DIR}/cmake
    ${CMAKE_INSTALL_PREFIX}/share
    ${CMAKE_CURRENT_LIST_DIR}/cmake
    $ENV{HOME}/.local/share )

# find Qt
if( WIN32 )
    set( QT_QMAKE_EXECUTABLE "$ENV{Qt_DIR}/bin/qmake.exe" )
endif()
find_package( Qt4 COMPONENTS QtCore QtGui QtXml )


# some more qt stuff
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# QCustomPlot (way smaller than QWT)
set( Q_CUSTOM_PLOT_INC extern/qcustomplot/qcustomplot.h )
set( Q_CUSTOM_PLOT_SRC extern/qcustomplot/qcustomplot.cpp )

# set the target
set( LimeS_Target limes )

# set the header files of QObject derived classes
set( LimeS_INC include/LimeS.hpp )
set( LimeS_SRC 
    src/LimeS.cpp
    src/main.cpp )
set( LimeS_QT_UI ui/LimeS.ui )
set( LimeS_QT_RES res/LimeS.qrc )

# set include directories
set( LimeS_INCLUDE_DIRS
    include
    extern/qcustomplot
    ${Lime_INCLUDE_DIRS}
    $ENV{HOME}/.local/include
    ${CMAKE_CURRENT_BINARY_DIR} )

# set link libraries
set( LimeS_LINK_LIBS
    ${QT_LIBRARIES}
    ${Lime_LIBRARY} )

# set compile definitions
set( LimeS_COMPILE_DEFINITIONS
    ${Lime_COMPILE_DEFINITIONS}
    QT_NO_DEPRECATED
    _USE_MATH_DEFINES )

# run the qt moc & uic
QT4_ADD_RESOURCES( LimeS_QT_RES_SRC ${LimeS_QT_RES})
QT4_WRAP_CPP( LimeS_QT_HPP_MOC
    ${LimeS_INC}
    ${Q_CUSTOM_PLOT_INC} )
QT4_WRAP_UI( LimeS_QT_UI_UIC ${LimeS_QT_UI} )

# set include paths
include_directories( ${LimeS_INCLUDE_DIRS} )

# add the exec
add_executable( ${LimeS_Target}
    ${LimeS_SRC}
    ${LimeS_INC}
    ${LimeS_QT_HPP_MOC}
    ${LimeS_QT_UI_UIC}
    ${LimeS_QT_RES_SRC}
    ${Q_CUSTOM_PLOT_INC}
    ${Q_CUSTOM_PLOT_SRC} )

# enable C++11 support
if( NOT WIN32 )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
    else( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Qunused-arguments")
    endif()
endif()

# set link libraries
target_link_libraries( ${LimeS_Target} ${LimeS_LINK_LIBS} )

# configure target
set_target_properties(${LimeS_Target}
    PROPERTIES
        PUBLIC_HEADER "${LimeS_INC}"
        COMPILE_DEFINITIONS "${LimeS_COMPILE_DEFINITIONS}"
        COMPILE_FLAGS "${Lime_COMPILE_FLAGS}"
        LINK_FLAGS "${Lime_LINK_FLAGS}" )
