program helloBPWriter
    use mpi
    use adios2f

    implicit none

    integer :: inx, irank, isize, ierr
    real, dimension(:), allocatable :: myArray


    call MPI_Init(ierr)
    call MPI_Comm_rank(MPI_COMM_WORLD, irank, ierr)
    call MPI_Comm_size(MPI_COMM_WORLD, isize, ierr)

    write(*,*) 'MPI rank ', irank
    write(*,*) 'MPI size ', isize

end program helloBPWriter
