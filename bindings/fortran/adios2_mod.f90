!
! Distributed under the OSI-approved Apache License, Version 2.0.  See
!  accompanying file Copyright.txt for details.
!
!  adios2_mod.f90 : ADIOS2 Fortran bindings central module
!
!   Created on: Mar 13, 2017
!       Author: William F Godoy godoywf@ornl.gov
!

module adios2

    use adios2_params

    implicit none

    interface

        subroutine adios2_init_mpi(adios, comm, debug_mode, ierr)
            integer, intent(out) :: adios
            integer, intent(in) :: comm
            integer, intent(in) :: debug_mode
            integer, intent(out) :: ierr
        end

        subroutine adios2_init_config_mpi(adios, config_file, comm,  &
            debug_mode, ierr)
            integer, intent(out) :: adios
            character(*), intent(in) :: config_file
            integer, intent(in) :: comm
            integer, intent(in) :: debug_mode
            integer, intent(out) :: ierr
        end

        subroutine adios2_declare_io(io, adios, io_name, ierr)
            integer, intent(out) :: io
            integer, intent(in) :: adios
            character(*), intent(in)  :: io_name
            integer, intent(out) :: ierr
        end

    end interface

end module
