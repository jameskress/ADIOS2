/*
 * adios2_f.cpp
 *
 *  Created on: Aug 14, 2017
 *      Author: William F Godoy
 */

#include <stdexcept>

#ifdef ADIOS2_USE_MPI_F
#include <mpi.h>
#endif

#include <adios2_c.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ADIOS2_USE_MPI_F
void adios2f_init_mpi(adios2_ADIOS *adios, int comm,
                      const adios2_debug_mode debug_mode, int *ierr)
{
    *ierr = 0;
    try
    {
        adios = adios2_init(MPI_Comm_f2c(comm), debug_mode);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}

void adios2f_init_config_mpi(adios2_ADIOS *adios, const char *config_file,
                             int comm, const adios2_debug_mode debug_mode,
                             int *ierr)
{
    *ierr = 0;
    try
    {
        adios = adios2_init_config(config_file, MPI_Comm_f2c(comm), debug_mode);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}
#else
void adios2f_init_nompi(adios2_ADIOS *adios, const adios2_debug_mode debug_mode,
                        int *ierr)
{
    *ierr = 0;
    try
    {
        adios = adios2_init_nompi(debug_mode);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}

void adios2f_init_config_nompi(adios2_ADIOS *adios, const char *config_file,
                               const adios2_debug_mode debug_mode, int *ierr)
{
    *ierr = 0;
    try
    {
        adios = adios2_init_config_nompi(config_file, debug_mode);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}
#endif

void adios2f_declare_io(adios2_IO *io, adios2_ADIOS *adios, const char *io_name,
                        int *ierr)
{
    *ierr = 0;
    try
    {
        io = adios2_declare_io(adios, io_name);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}

#ifdef __cplusplus
}
#endif
