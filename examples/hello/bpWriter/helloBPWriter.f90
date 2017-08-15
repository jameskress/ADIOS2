program helloBPWriter
    use mpi
    use adios2

    implicit none

    integer :: inx, irank, isize, ierr
    integer :: adios, io, var1, engine1
    real, dimension(:), allocatable :: myArray


    call MPI_Init(ierr)
    call MPI_Comm_rank(MPI_COMM_WORLD, irank, ierr)
    call MPI_Comm_size(MPI_COMM_WORLD, isize, ierr)

    call adios2_init_mpi(adios, MPI_COMM_WORLD, 0, ierr)
    call adios2_declare_io(io, adios, "myIO", ierr)

    write(*,*) 'MPI rank ', irank, ' size ', isize


    call MPI_Finalize(ierr)

end program helloBPWriter
