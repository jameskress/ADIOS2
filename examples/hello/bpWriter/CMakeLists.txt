#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

if(ADIOS2_HAVE_MPI)
  find_package(MPI COMPONENTS C REQUIRED)

  add_executable(hello_bpWriter helloBPWriter.cpp)
  target_include_directories(hello_bpWriter PRIVATE ${MPI_C_INCLUDE_PATH})
  target_link_libraries(hello_bpWriter ${MPI_C_LIBRARIES})
  
else()
  add_executable(hello_bpWriter helloBPWriter_nompi.cpp)
  
endif()

target_link_libraries(hello_bpWriter adios2)
