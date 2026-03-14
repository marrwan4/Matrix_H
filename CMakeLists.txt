cmake_minimum_required(VERSION 3.10)
project(Matrix_H)

# Manually point CMake to the MS-MPI environment variables
set(MPI_C_HEADER_DIR "$ENV{MSMPI_INC}")
set(MPI_CXX_HEADER_DIR "$ENV{MSMPI_INC}")
set(MPI_C_LIBRARIES "$ENV{MSMPI_LIB64}/msmpi.lib")
set(MPI_CXX_LIBRARIES "$ENV{MSMPI_LIB64}/msmpi.lib")

find_package(MPI REQUIRED)

add_executable(Matrix_H Src/Matrix.h)
target_link_libraries(Matrix_H PRIVATE MPI::MPI_CXX)
