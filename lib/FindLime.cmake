#  Try to find Lime
#
#  Lime_FOUND - System has Lime
#  Lime_INCLUDE_DIRS - The Lime include directories
#  Lime_LIBRARY - The libraries needed to use Lime
#  Lime_DEFINITIONS - Compiler switches required for using Lime


# try to find the include dir
find_path( Lime_INCLUDE_DIR 
    NAMES
        lime/Segmentation.hpp
        lime/util.hpp
    PATHS
        $ENV{IRIS_DIR}/include
        ${CMAKE_INSTALL_PREFIX}/include
        /usr/include
        /usr/local/include
        /opt/include
        /opt/local/include
    PATH_SUFFIXES
        lime )
        
# find the path to the exported lib files
find_file( Lime_DEPENDS Lime.cmake
    PATHS
        ${CMAKE_INSTALL_PREFIX}/share
        /usr/share
        /usr/local/share
        /opt/share
        /opt/local/share )

# load all the targets
if( Lime_DEPENDS )
    include( "${Lime_DEPENDS}" )
else()
    MESSAGE( FATAL_ERROR "Lime: not installed.")
endif()

# check if this is a valid component
set( Lime_LIBRARY lime )
if( TARGET ${Lime_LIBRARY} )
    # include the component
    list( APPEND Lime_LIBRARIES ${Lime_LIBRARY} )
    MESSAGE( STATUS "Lime found.")
else()
    MESSAGE( FATAL_ERROR "Lime target not available.")
endif()


# set the include dirs
set( Lime_INCLUDE_DIRS 
    ${Lime_INCLUDE_DIR}
    ${Lime_INCLUDE_DIR}/lime )
    
    


