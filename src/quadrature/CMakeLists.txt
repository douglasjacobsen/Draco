#-----------------------------*-cmake-*----------------------------------------#
# file   quadrature/CMakeLists.txt
# brief  Instructions for building quadrature Makefiles.
# note   Copyright (C) 2010-2020 Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.17.0)
project( quadrature CXX )

# Local variables -------------------------------------------------------------#
string(CONCAT header_prefix_source "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${DBSCFG_IMPORT_PREFIX}/quadrature>")

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #
file( GLOB sources RELATIVE "${PROJECT_SOURCE_DIR}" *.cc )
file( GLOB headers RELATIVE "${PROJECT_SOURCE_DIR}" *.hh )
file( GLOB f90sources RELATIVE "${PROJECT_SOURCE_DIR}" *.f90 )

# prefix header files with appropriate paths (build vs install).
foreach( file ${headers} )
  list(APPEND headers_public ${header_prefix_source}/${file})
endforeach()

# ---------------------------------------------------------------------------- #
# Build package library
# ---------------------------------------------------------------------------- #
add_component_library(
   TARGET       Lib_quadrature
   TARGET_DEPS  "Lib_special_functions;Lib_parser;Lib_mesh_element"
   LIBRARY_NAME ${PROJECT_NAME} )
target_sources( Lib_quadrature PRIVATE "${headers_public}" "${sources}" )
set_target_properties(Lib_quadrature PROPERTIES
  PUBLIC_HEADER "${headers_public}")

# ---------------------------------------------------------------------------- #
# Installation instructions
# ---------------------------------------------------------------------------- #
install( TARGETS Lib_quadrature EXPORT draco-targets
  LIBRARY DESTINATION "${DBSCFGDIR_LIBRARY}"
  ARCHIVE DESTINATION "${DBSCFGDIR}lib"
  RUNTIME DESTINATION "${DBSCFGDIR}bin"
  PUBLIC_HEADER DESTINATION "${DBSCFGDIR}include/quadrature" )
install( FILES ${f90sources}
  DESTINATION ${DBSCFGDIR}include/quadrature )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  install(FILES $<TARGET_PDB_FILE:Lib_quadrature> DESTINATION ${DBSCFGDIR}bin
    OPTIONAL)
endif()

# ---------------------------------------------------------------------------- #
# Unit tests
# ---------------------------------------------------------------------------- #
if( BUILD_TESTING )
   add_subdirectory( test )

  if( HAVE_Fortran )
    # For Unix, this will simply run add_subdirectory(ftest). For MVSE or XCode,
    # an ExternalProject will be created that uses an alternate gfortran via
    # Makefiles to generate the library external to the main project.
    include(CMakeAddFortranSubdirectory)

    # CMake does not support storing a list of lists when sending data to a
    # macro.  Because Draco_TPL_INCLUDE_DIRS is a list and we want to stuff it
    # into the list build_system_state, recode Draco_TPL_INCLUDE_DIRS by
    # replacing semicolons with triple underscores.  The list will be
    # reconstructed in the subdirectory's CMakeLists.txt.
    string( REGEX REPLACE ";" "___" tmp
      "${Draco_TPL_INCLUDE_DIRS};${MPI_Fortran_INCLUDE_PATH}")

    # The alternate build system (Makefiles if we are Apple/OSX or Linux/Ninja)
    # will need some of the current build system parameters:
    set( draco_DIR ${Draco_SOURCE_DIR}/config )
    set( build_system_state
      "-DDRACO_C4=${DRACO_C4}"
      "-DDRACO_LIBRARY_TYPE=${DRACO_LIBRARY_TYPE}"
      "-DDraco_TPL_INCLUDE_DIRS=${tmp}"
      "-Ddraco_DIR=${draco_DIR}")
    if( ${DRACO_C4} MATCHES "MPI" )
      list( APPEND build_system_state
      "-DMPI_C_LIBRARIES=${MPI_C_LIBRARIES}"
      "-DMPI_C_INCLUDE_DIRS=${MPI_C_INCLUDE_DIRS}" )
    endif()
    if( WIN32 )
      # For Win32 builds, DLL and applications are built in the directory
      # specified by CMAKE_RUNTIME_OUTPUT_DIRECTORY.
      list( APPEND build_system_state
        "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${Draco_BINARY_DIR}/\${CMAKE_BUILD_TYPE}" )
      if(CMAKE_TOOLCHAIN_FILE)
        list( APPEND build_system_state
          "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
      endif()
    else()
      list( APPEND build_system_state "-DHAVE_CUDA=${HAVE_CUDA}" )
    endif()

    cmake_add_fortran_subdirectory(
      ftest                        # directory name
      PROJECT quadrature_ftest  # project name in top level CMakeLists.txt
      # Creates target named 'quadrature_ftest_build'
      ARCHIVE_DIR  ftest          # .lib location relative to root binary tree
      RUNTIME_DIR  ftest          # .dll location relative to root binary tree
      LIBRARIES    rtt_quadrature_ftest # new library will have this name.
                                  # --> librtt_quadrature_ftest.dll.
      TARGET_NAMES Lib_quadrature_ftest
      DEPENDS      "Lib_quadrature_test;Lib_quadrature;Lib_c4;Lib_dsxx"
      NO_EXTERNAL_INSTALL
      CMAKE_COMMAND_LINE "${build_system_state}"
      # VERBOSE
      )

    # This directory contains c++ main code that wraps the Fortran tests.
    add_subdirectory( fctest )
  endif()

endif()

# ---------------------------------------------------------------------------- #
# Autodoc
# ---------------------------------------------------------------------------- #
process_autodoc_pages()

#------------------------------------------------------------------------------#
# End quadrature/CMakeLists.txt
#------------------------------------------------------------------------------#
