program helloBPWriter

    use adios2

    implicit none

    integer :: inx, ierr
    integer(kind=8) :: adios, io
    real, dimension(:), allocatable :: myArray

    inx = 10

    call adios2_init_nompi(adios, 0, ierr)

    write(*,*) "Nx : ", inx
    write(*,*) "ADIOS address :", adios

    call adios2_declare_io(io, adios, "myIO", ierr)

end program helloBPWriter
