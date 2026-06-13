cmake_minimum_required(VERSION 3.14)
project(MatrixLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Define the core template library (Header-Only)
add_library(matrix_lib INTERFACE)

# 2. Set include directory paths
target_include_directories(matrix_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Src>
)

# 3. Optional OpenMP detection
find_package(OpenMP)
if(OpenMP_FOUND)
    target_link_libraries(matrix_lib INTERFACE OpenMP::OpenMP_CXX)
    target_compile_definitions(matrix_lib INTERFACE USE_OPENMP)
endif()

# 4. Optional MPI detection (Graceful fallback if missing)
find_package(MPI)
if(MPI_FOUND)
    target_link_libraries(matrix_lib INTERFACE MPI::MPI_CXX)
    target_compile_definitions(matrix_lib INTERFACE USE_MPI)
else()
    # If MS-MPI wasn't auto-found but variables exist, manual fallback here
    if(DEFINED ENV{MSMPI_INC})
        target_include_directories(matrix_lib INTERFACE $ENV{MSMPI_INC})
        target_link_libraries(matrix_lib INTERFACE $ENV{MSMPI_LIB64}/msmpi.lib)
        target_compile_definitions(matrix_lib INTERFACE USE_MPI)
    endif()
endif()

# 5. The Test Runner Application
add_executable(MatrixProgram Src/MatrixProgram.cpp)
target_link_libraries(MatrixProgram PRIVATE matrix_lib)