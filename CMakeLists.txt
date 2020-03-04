cmake_minimum_required(VERSION 3.10)

# Basic project setup
project(aosoa_storage CXX)

##############################################################################
# Options                                                                    #
##############################################################################

option(STORAGE_ENABLE_BENCHMARK "Enable benchmark" On)

option(STORAGE_ENABLE_TEST "Enable Testing" On)

option(STORAGE_ENABLE_OPENMP "Enable OpenMP" On)

option(STORAGE_ENABLE_CUDA "Enable Cuda" Off)

##############################################################################
# Settings                                                                   #
##############################################################################

if(${STORAGE_ENABLE_CUDA})
  # set(CMAKE_CXX_COMPILER ${CUDA_COMPILER})
else()
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# CMake Modules
include(cmake/download_project.cmake)

##############################################################################
# External Libraries                                                         #
##############################################################################

# Kokkos
download_project(
  PROJ Kokkos
  GIT_REPOSITORY https://github.com/kokkos/kokkos.git
  GIT_TAG 3.0.00
  UPDATE_DISCONNECTED 1
)

# Enable Kokkos devices
set(Kokkos_ENABLE_SERIAL On CACHE INTERNAL "")
if(${STORAGE_ENABLE_CUDA})
  set(Kokkos_ENABLE_CUDA On CACHE INTERNAL "")
elseif(${STORAGE_ENABLE_OPENMP})
  set(Kokkos_ENABLE_OPENMP On CACHE INTERNAL "")
endif()

add_subdirectory(${Kokkos_SOURCE_DIR} ${Kokkos_BINARY_DIR})

set(Kokkos_DEVICES SERIAL)
if(${STORAGE_ENABLE_CUDA})
  set(Kokkos_DEVICES CUDA ${Kokkos_DEVICES})
elseif(${STORAGE_ENABLE_OPENMP})
  set(Kokkos_DEVICES OPENMP ${Kokkos_DEVICES})
endif()

# Cabana
download_project(
  PROJ Cabana
  GIT_REPOSITORY https://github.com/Liby99/Cabana # Forked to get rid of find_package(Kokkos)
  UPDATE_DISCONNECTED 1
)
add_subdirectory(${Cabana_SOURCE_DIR} ${Cabana_BINARY_DIR})

##############################################################################
# Core Library                                                               #
##############################################################################

add_library(aosoa_storage INTERFACE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(aosoa_storage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(aosoa_storage INTERFACE Kokkos::kokkos cabanacore)

##############################################################################
# Tests                                                                      #
##############################################################################

if(${STORAGE_ENABLE_TEST})
  # Testing setup
  enable_testing()
  file(GLOB_RECURSE test_files "tests/**.cpp")
  foreach(test_file ${test_files})
    get_filename_component(test_target ${test_file} NAME_WE)
    add_executable(${test_target} ${test_file})
    target_include_directories(${test_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
    target_link_libraries(${test_target} PUBLIC Kokkos::kokkos cabanacore)
    add_test(${test_target} ${test_target})
  endforeach()
endif()

##############################################################################
# Benchmarks                                                                 #
##############################################################################

if(${STORAGE_ENABLE_BENCHMARK})
  file(GLOB_RECURSE bench_files "benchmarks/**.cpp")
  foreach(bench_file ${bench_files})
    get_filename_component(bench_target ${bench_file} NAME_WE)
    add_executable(${bench_target} ${bench_file})
    target_include_directories(${bench_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
    target_link_libraries(${bench_target} PUBLIC Kokkos::kokkos cabanacore)
    add_test(${bench_target} ${bench_target})
  endforeach()
endif()