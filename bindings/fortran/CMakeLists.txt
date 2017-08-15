#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

set(adios2f_targets)

if(ADIOS2_HAVE_MPI)
  add_library(adios2f adios2f_mod.f90 adios2f.cpp)
else()
  add_library(adios2f adios2f_nompi_mod.f90 adios2f.cpp)
endif()  
  
target_link_libraries(adios2f PRIVATE adios2 adios2_c)

if(ADIOS2_HAVE_MPI)
  find_package(MPI COMPONENTS C REQUIRED)
  find_package(MPI COMPONENTS Fortran REQUIRED)
  target_compile_definitions(adios2f PUBLIC ADIOS2_HAVE_MPI_F)
  target_include_directories(adios2f PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_C_INCLUDE_PATH})
  target_link_libraries(adios2f PRIVATE ${MPI_Fortran_LIBRARIES} ${MPI_C_LIBRARIES} )
else()
  target_include_directories(adios2f PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()

list(APPEND adios2f_targets adios2f)

install(
  TARGETS ${adios2f_targets} EXPORT adios2
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)