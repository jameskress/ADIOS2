!
! Distributed under the OSI-approved Apache License, Version 2.0.  See
!  accompanying file Copyright.txt for details.
!
!  adios2_mod.f90 : ADIOS2 Fortran bindings central module
!
!   Created on: Mar 13, 2017
!       Author: William F Godoy godoywf@ornl.gov
!
module adios2_params
    implicit none

    ! Debug mode
    integer, parameter :: adios2_debug_mode_on = 0
    integer, parameter :: adios2_debug_mode_off = 1

    ! Type
    integer, parameter :: adios2_type_character = 0
    integer, parameter :: adios2_type_integer = 1
    integer, parameter :: adios2_type_real = 2
    integer, parameter :: adios2_type_real8 = 3
    integer, parameter :: adios2_type_double_precision = 3
    integer, parameter :: adios2_type_complex = 4
    integer, parameter :: adios2_type_complex8 = 5

    integer, parameter :: adios2_type_integer1 = 6
    integer, parameter :: adios2_type_integer2 = 7
    integer, parameter :: adios2_type_integer4 = 8
    integer, parameter :: adios2_type_integer8 = 9

    ! Open Mode
    integer, parameter :: adios2_open_mode_undefined = 0
    integer, parameter :: adios2_open_mode_write = 1
    integer, parameter :: adios2_open_mode_read = 2
    integer, parameter :: adios2_open_mode_append = 3
    integer, parameter :: adios2_open_mode_read_write = 4

end module
