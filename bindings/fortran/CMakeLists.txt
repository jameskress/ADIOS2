#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

set(adios2_f_targets)

if(ADIOS2_HAVE_MPI)
  add_library(adios2_f adios2_mod.f90 adios2_params_mod.f90 adios2_f.cpp)
else()
  add_library(adios2_f adios2_nompi_mod.f90 adios2_params_mod.f90 adios2_f.cpp)
endif()  
  
target_link_libraries(adios2_f PRIVATE adios2 adios2_c)

if(ADIOS2_HAVE_MPI)
  find_package(MPI COMPONENTS C REQUIRED)
  find_package(MPI COMPONENTS Fortran REQUIRED)
  target_compile_definitions(adios2_f PUBLIC ADIOS2_HAVE_MPI_F)
  target_include_directories(adios2_f PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_C_INCLUDE_PATH} ${CMAKE_CURRENT_BINARY_DIR})
  target_link_libraries(adios2_f PRIVATE ${MPI_C_LIBRARIES} ${MPI_Fortran_LIBRARIES})
else()
  target_include_directories(adios2_f PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
endif()

list(APPEND adios2_f_targets adios2_f)

install(
  TARGETS ${adios2_f_targets} EXPORT adios2
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)