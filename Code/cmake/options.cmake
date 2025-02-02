# This file is part of HemeLB and is Copyright (C)
# the HemeLB team and/or their institutions, as detailed in the
# file AUTHORS. This software is provided under the terms of the
# license in the file LICENSE.

# This file defines all the CMake options and cache variables needed
# by HemeLB (main application). It can be included from either the the
# master-build CMakeLists.txt or the app CMakeLists.txt

# Private: list of options
SET(_HEMELB_OPTIONS "")

#
# Declare an option (ON/OFF value)
#
macro(hemelb_option NAME DESC DEFAULT)
  option(${NAME} ${DESC} ${DEFAULT})
  list(APPEND _HEMELB_OPTIONS ${NAME})
endmacro()

# Private: list of the cached variables
SET(_HEMELB_CACHEVARS "")

#
# Declare a cache variable
#
macro(hemelb_cachevar NAME DEFAULT TYPE DESC)
  set(${NAME} ${DEFAULT} CACHE ${TYPE} ${DESC})
  list(APPEND _HEMELB_CACHEVARS ${NAME})
endmacro()

# Private: list of variables to be forwarded from the master to the
# code builds
SET(_HEMELB_FWDVARS "")

#
# Declare a variable to be forwarded from master build to code build
#
macro(hemelb_fwdvar NAME)
  list(APPEND _HEMELB_FWDVARS ${NAME})
endmacro()

#
# Function to produce the CMake defines to pass data from master to
# code runs of cmake.
#
function(hemelb_pass_cmake_defines output)
  set(ans "")

  foreach(OPT_NAME ${_HEMELB_OPTIONS})
    list(APPEND ans "-D${OPT_NAME}=${${OPT_NAME}}")
  endforeach()

  foreach(V_NAME ${_HEMELB_CACHEVARS})
    list(APPEND ans "-D${V_NAME}=${${V_NAME}}")
  endforeach()

  foreach(F_NAME ${_HEMELB_FWDVARS})
    if (DEFINED ${F_NAME})
      list(APPEND ans "-D${F_NAME}=${${F_NAME}}")
    endif()
  endforeach()
  set(${output} ${ans} PARENT_SCOPE)
endfunction()


#
# Specify the options
#
hemelb_option(HEMELB_BUILD_DEBUGGER "Build the built in debugger" ON)
hemelb_option(HEMELB_VALIDATE_GEOMETRY "Validate geometry" OFF)
hemelb_option(HEMELB_BUILD_MAIN "Build HemeLB executable" ON)
hemelb_option(HEMELB_BUILD_MULTISCALE "Build HemeLB Multiscale functionality" OFF)
hemelb_option(HEMELB_BUILD_TESTS_ALL "Build all the tests" ON)
hemelb_option(HEMELB_BUILD_TESTS_UNIT "Build the unit-tests (HEMELB_BUILD_TESTS_ALL takes precedence)" ON)
hemelb_option(HEMELB_BUILD_TESTS_FUNCTIONAL "Build the functional tests (HEMELB_BUILD_TESTS_ALL takes precedence)" ON)
hemelb_option(HEMELB_USE_ALL_WARNINGS_GNU "Show all compiler warnings on development builds (gnu-style-compilers)" ON)
hemelb_option(HEMELB_USE_STREAKLINES "Calculate streakline images" OFF)
hemelb_option(HEMELB_DEPENDENCIES_SET_RPATH "Set runtime RPATH" ON)
hemelb_option(HEMELB_STATIC_ASSERT "Use simple compile-time assertions" ON)
hemelb_option(HEMELB_WAIT_ON_CONNECT "Wait for steering client" OFF)
hemelb_option(HEMELB_IMAGES_TO_NULL "Write images to null" OFF)
hemelb_option(HEMELB_USE_SSE3 "Use SSE3 intrinsics" ON)
hemelb_option(HEMELB_USE_VELOCITY_WEIGHTS_FILE "Use Velocity weights file" OFF)
hemelb_option(HEMELB_SEPARATE_CONCERNS "Communicate for each concern separately" OFF)
hemelb_option(HEMELB_LATTICE_INCOMPRESSIBLE "Use an incompressible lattice" OFF)
hemelb_option(HEMELB_CUDA_AWARE_MPI "Use CUDA-aware MPI" ON)

#
# Specify the variables
#
hemelb_cachevar(HEMELB_EXECUTABLE "hemelb"
  STRING "File name of executable to produce")
hemelb_cachevar(HEMELB_READING_GROUP_SIZE 5
  INTEGER "Number of cores to use to read geometry file.")
hemelb_cachevar(HEMELB_LOG_LEVEL Info
  STRING "Log level, choose 'Critical', 'Error', 'Warning', 'Info', 'Debug' or 'Trace'" )
hemelb_cachevar(HEMELB_STEERING_LIB none
  STRING "Steering library, choose 'basic' or 'none'" )
hemelb_cachevar(HEMELB_DEPENDENCIES_PATH "${HEMELB_ROOT_DIR}/dependencies"
  FILEPATH "Path to find dependency find modules")
hemelb_cachevar(HEMELB_DEPENDENCIES_INSTALL_PATH ${HEMELB_DEPENDENCIES_PATH}
  FILEPATH "Path to find dependency includes and libraries")
hemelb_cachevar(HEMELB_SUBPROJECT_MAKE_JOBS 1
  INTEGER "Number of jobs to use for subproject build steps")
hemelb_cachevar(HEMELB_OPTIMISATION "-O3"
  STRING "Optimisation level (can be blank or -O1 to -O4)")
hemelb_cachevar(HEMELB_PROFILING ""
 STRING "Profiling options (can be blank or something like -g -pg)")

hemelb_cachevar(HEMELB_LATTICE "D3Q15"
  STRING "Select the lattice type to use (D3Q15,D3Q19,D3Q27)")
hemelb_cachevar(HEMELB_KERNEL "LBGK"
  STRING "Select the kernel to use (LBGK,EntropicAnsumali,EntropicChik,MRT,TRT,NNCY,NNCYMOUSE,NNC,NNTPL)")
hemelb_cachevar(HEMELB_WALL_BOUNDARY "SIMPLEBOUNCEBACK"
  STRING "Select the boundary conditions to be used at the walls (BFL,GZS,SIMPLEBOUNCEBACK,JUNKYANG)")
hemelb_cachevar(HEMELB_INLET_BOUNDARY "NASHZEROTHORDERPRESSUREIOLET"
  STRING "Select the boundary conditions to be used at the inlet (NASHZEROTHORDERPRESSUREIOLET,LADDIOLET)")
hemelb_cachevar(HEMELB_OUTLET_BOUNDARY "NASHZEROTHORDERPRESSUREIOLET"
  STRING "Select the boundary conditions to be used at the outlets (NASHZEROTHORDERPRESSUREIOLET,LADDIOLET)")
hemelb_cachevar(HEMELB_COMPUTE_ARCHITECTURE "AMDBULLDOZER"
  STRING "Select the architecture of the machine being used (INTELSANDYBRIDGE,AMDBULLDOZER,NEUTRAL,ISBFILEVELOCITYINLET)")
hemelb_cachevar(HEMELB_WALL_INLET_BOUNDARY "NASHZEROTHORDERPRESSURESBB"
  STRING "Select the boundary conditions to be used at corners between walls and inlets (NASHZEROTHORDERPRESSURESBB,NASHZEROTHORDERPRESSUREBFL,LADDIOLETSBB,LADDIOLETBFL)")
hemelb_cachevar(HEMELB_WALL_OUTLET_BOUNDARY "NASHZEROTHORDERPRESSURESBB"
  STRING "Select the boundary conditions to be used at corners between walls and outlets (NASHZEROTHORDERPRESSURESBB,NASHZEROTHORDERPRESSUREBFL,LADDIOLETSBB,LADDIOLETBFL)")
hemelb_cachevar(HEMELB_POINTPOINT_IMPLEMENTATION Coalesce
  STRING "Point to point comms implementation, choose 'Coalesce', 'Separated', or 'Immediate'" )
hemelb_cachevar(HEMELB_GATHERS_IMPLEMENTATION Separated
  STRING "Gather comms implementation, choose 'Separated', or 'ViaPointPoint'" )
hemelb_cachevar(HEMELB_ALLTOALL_IMPLEMENTATION Separated
  STRING "Alltoall comms implementation, choose 'Separated', or 'ViaPointPoint'" )

#
# Specify the variables requiring forwarding
#
hemelb_fwdvar(CMAKE_INSTALL_PREFIX)
hemelb_fwdvar(CMAKE_C_COMPILER)
hemelb_fwdvar(CMAKE_CXX_COMPILER)

hemelb_fwdvar(MPI_C_COMPILER)
hemelb_fwdvar(MPI_CXX_COMPILER)
hemelb_fwdvar(MPI_C_NO_INTERROGATE)
hemelb_fwdvar(MPI_CXX_NO_INTERROGATE)
hemelb_fwdvar(MPI_C_LIBRARIES)
hemelb_fwdvar(MPI_C_INCLUDE_PATH)

hemelb_fwdvar(BOOST_ROOT)
hemelb_fwdvar(CTEMPLATE_USE_STATIC)
hemelb_fwdvar(CPPUNIT_USE_STATIC)
hemelb_fwdvar(METIS_INCLUDE_DIR)
hemelb_fwdvar(METIS_LIBRARY)
hemelb_fwdvar(PARMETIS_INCLUDE_DIR)
hemelb_fwdvar(PARMETIS_LIBRARY)
hemelb_fwdvar(TINYXML_INCLUDE_DIR)
hemelb_fwdvar(TINYXML_LIBRARIES)
