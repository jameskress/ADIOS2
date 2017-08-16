/*
 * adios2_f.cpp
 *
 *  Created on: Aug 14, 2017
 *      Author: William F Godoy
 */

#include "adios2_f.h"

#include <iostream>
#include <stdexcept>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ADIOS2_HAVE_MPI_F
void adios2_init_(adios2_ADIOS *adios, int comm,
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

void adios2_init_config_(adios2_ADIOS *adios, const char *config_file, int comm,
                         const adios2_debug_mode debug_mode, int *ierr)
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
void adios2_init_nompi_(adios2_ADIOS **adios, const int debug_mode, int *ierr)
{
    *ierr = 0;
    try
    {
        *adios = adios2_init_nompi(static_cast<adios2_debug_mode>(debug_mode));
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
    std::cout << "Created adios address " << adios << "\n";
}

void adios2_init_config_nompi_(adios2_ADIOS **adios, const char *config_file,
                               const int debug_mode, int *ierr)
{
    *ierr = 0;
    try
    {
        *adios = adios2_init_config_nompi(
            config_file, static_cast<adios2_debug_mode>(debug_mode));
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }
}
#endif

void adios2_declare_io_(adios2_IO **io, adios2_ADIOS **adios,
                        const char *io_name, int *ierr)
{
    *ierr = 0;
    try
    {
        *io = adios2_declare_io(*adios, io_name);
    }
    catch (std::exception &e)
    {
        *ierr = 1;
    }

    std::cout << "Declaring IO: " << io_name << "\n";
}

#ifdef __cplusplus
}
#endif
