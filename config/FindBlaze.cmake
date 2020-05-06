#.rst:
# FindLibquo
# ---------
#
# Find Libquo includes and libraries.
#
# BLAZE (as in "status quo") is a runtime library that aids in accommodating
# thread-level heterogeneity in dynamic, phased MPI+X applications comprising
# single- and multi-threaded libraries.
#
# https://github.com/lanl/libquo
#
# Imported Targets
# ^^^^^^^^^^^^^^^^
#
# If Libquo is found, this module defines the following :prop_tgt:`IMPORTED`
# targets::
#
#  BLAZE::libquo         - The Libquo library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project::
#
#  BLAZE_FOUND          - True if Libquo found on the local system
#  BLAZE_INCLUDE_DIRS   - Location of Libquo header files.
#  BLAZE_LIBRARIES      - The Libquo libraries.
#  BLAZE_VERSION        - The version of the discovered Libquo install.
#
# Hints
# ^^^^^
#
# Set ``BLAZE_ROOT_DIR`` or ``BLAZE_HOME`` to a directory that contains a Libquo
# installation.
#
# This script expects to find libraries at ``$BLAZE_ROOT_DIR/lib`` and the
# Libquo headers at ``$BLAZE_ROOT_DIR/include``.  The library directory may
# optionally provide Release and Debug folders.
#
# Cache Variables
# ^^^^^^^^^^^^^^^
#
# This module may set the following variables depending on platform and type of
# Libquo installation discovered.  These variables may optionally be set to help
# this module find the correct files::
#
#  BLAZE_LIBRARY        - Location of the Libquo library.
#  BLAZE_LIBRARY_DEBUG  - Location of the debug Libquo library (if any).
#

#=============================================================================
# Copyright (C) 2019-2020 Triad National Security, LLC., All rights reserved.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Include these modules to handle the QUIETLY and REQUIRED arguments.
include(FindPackageHandleStandardArgs)

#=============================================================================
# If the user has provided ``BLAZE_ROOT_DIR``, use it!  Choose items found
# at this location over system locations.
if( EXISTS "$ENV{BLAZE_ROOT_DIR}" )
  file( TO_CMAKE_PATH "$ENV{BLAZE_ROOT_DIR}" BLAZE_ROOT_DIR )
elseif( EXISTS "$ENV{BLAZE_HOME}" )
  file( TO_CMAKE_PATH "$ENV{BLAZE_HOME}" BLAZE_ROOT_DIR )
endif()
if( BLAZE_ROOT_DIR )
  set( BLAZE_ROOT_DIR "${BLAZE_ROOT_DIR}" CACHE PATH
    "Prefix for Libquo installation." )
endif()

#=============================================================================
# Set BLAZE_INCLUDE_DIRS and BLAZE_LIBRARIES. Try to find the libraries at
# $BLAZE_ROOT_DIR (if provided) or in standard system locations.  These
# find_library and find_path calls will prefer custom locations over standard
# locations (HINTS).  If the requested file is not found at the HINTS location,
# standard system locations will be still be searched (/usr/lib64 (Redhat),
# lib/i386-linux-gnu (Debian)).

find_path( BLAZE_INCLUDE_DIR
  NAMES Blaze.h
  HINTS ${BLAZE_ROOT_DIR}/include ${BLAZE_INCLUDE_DIR}
  PATH_SUFFIXES Release Debug
)
#find_library( BLAZE_LIBRARY
#  NAMES Blaze
#  HINTS ${BLAZE_ROOT_DIR}/lib ${BLAZE_LIBDIR}
#  PATH_SUFFIXES Release Debug
#)
# Do we also have debug versions?
#find_library( BLAZE_LIBRARY_DEBUG
#  NAMES quo
#  HINTS ${BLAZE_ROOT_DIR}/lib ${BLAZE_LIBDIR}
#  PATH_SUFFIXES Debug
#)
set( BLAZE_INCLUDE_DIRS ${BLAZE_INCLUDE_DIR} )
#set( BLAZE_LIBRARIES ${BLAZE_LIBRARY} )

# Try to find the version.
#if( NOT BLAZE_VERSION )
#  if( EXISTS "${BLAZE_LIBDIR}/pkgconfig/libquo.pc" )
#    file( STRINGS "${BLAZE_LIBDIR}/pkgconfig/libquo.pc" libquo_version
#        REGEX "Version:" )
#    string( REGEX REPLACE ".*([0-9]+).([0-9]+)" "\\1" BLAZE_MAJOR ${libquo_version} )
#    string( REGEX REPLACE ".*([0-9]+).([0-9]+)" "\\2" BLAZE_MINOR ${libquo_version} )
##    string( REGEX REPLACE ".*([0-9]+)" "\\1" BLAZE_SUBMINOR ${libquo_h_subminor} )
#  endif()
#  # We might also try scraping the directory name for a regex match
#  # "libquo-X.X.X"
#endif()

#=============================================================================
# handle the QUIETLY and REQUIRED arguments and set BLAZE_FOUND to TRUE if
# all listed variables are TRUE.
find_package_handle_standard_args( Blaze
  FOUND_VAR
    Blaze_FOUND
  REQUIRED_VARS
    BLAZE_INCLUDE_DIR
#  VERSION_VAR
#    BLAZE_VERSION
    )

mark_as_advanced( BLAZE_ROOT_DIR BLAZE_VERSION BLAZE_INCLUDE_DIR)

#=============================================================================
# Register imported libraries:
# 1. If we can find a Windows .dll file (or if we can find both Debug and
#    Release libraries), we will set appropriate target properties for these.
# 2. However, for most systems, we will only register the import location and
#    include directory.

# Look for dlls, or Release and Debug libraries.
#if(WIN32)
  #string( REPLACE ".lib" ".dll" BLAZE_LIBRARY_DLL
    #"${BLAZE_LIBRARY}" )
  #string( REPLACE ".lib" ".dll" BLAZE_LIBRARY_DEBUG_DLL
    #"${BLAZE_LIBRARY_DEBUG}" )
#endif()

if( Blaze_FOUND AND NOT TARGET BLAZE::Blaze )
    #  if( EXISTS "${BLAZE_LIBRARY_DLL}" )
    #
    #    # Windows systems with dll libraries.
    #    add_library( BLAZE::libquo SHARED IMPORTED )
    #
    #    # Windows with dlls, but only Release libraries.
    #    set_target_properties( BLAZE::libquo PROPERTIES
    #      IMPORTED_LOCATION_RELEASE         "${BLAZE_LIBRARY_DLL}"
    #      IMPORTED_IMPLIB                   "${BLAZE_LIBRARY}"
    #      INTERFACE_INCLUDE_DIRECTORIES     "${BLAZE_INCLUDE_DIRS}"
    #      IMPORTED_CONFIGURATIONS           Release
    #      IMPORTED_LINK_INTERFACE_LANGUAGES "C" )
    #
    #    # If we have both Debug and Release libraries
    #    if( EXISTS "${BLAZE_LIBRARY_DEBUG_DLL}" )
    #      set_property( TARGET BLAZE::libquo APPEND PROPERTY
    #        IMPORTED_CONFIGURATIONS Debug )
    #      set_target_properties( BLAZE::libquo PROPERTIES
    #        IMPORTED_LOCATION_DEBUG           "${BLAZE_LIBRARY_DEBUG_DLL}"
    #        IMPORTED_IMPLIB_DEBUG             "${BLAZE_LIBRARY_DEBUG}" )
    #    endif()
    #
    #  else()

    # For all other environments (ones without dll libraries), create the
    # imported library targets.
    message("Found Blaze at ${BLASE_INCLUDE_DIR}")
    add_library( BLAZE::Blaze    UNKNOWN IMPORTED )
    set_target_properties( BLAZE::Blaze PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES     "${BLAZE_INCLUDE_DIRS}"
      IMPORTED_LINK_INTERFACE_LANGUAGES "C" )
  #  endif()
endif()

#------------------------------------------------------------------------------#
# End pkg_config/FindLibquo.cmake
#------------------------------------------------------------------------------#
