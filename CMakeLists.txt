cmake_minimum_required(VERSION 3.0)

project(mpi)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Use imported targets would make things much eazier. Thanks Levi for pointing it out.
add_executable(mpi mpi.c)
target_link_libraries(mpi ${MPI_C_LIBRARIES})