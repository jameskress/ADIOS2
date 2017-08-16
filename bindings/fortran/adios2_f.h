/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * adios2_f.h
 *
 *  Created on: Aug 15, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef BINDINGS_FORTRAN_ADIOS2_F_H_
#define BINDINGS_FORTRAN_ADIOS2_F_H_

#include <stddef.h>

#ifdef ADIOS2_USE_MPI_F
#include <mpi.h>
#endif

#include <adios2_c.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ADIOS2_HAVE_MPI_F
void adios2_init_(adios2_ADIOS *adios, int comm,
                  const adios2_debug_mode debug_mode, int *ierr);

void adios2_init_config_(adios2_ADIOS *adios, const char *config_file, int comm,
                         const adios2_debug_mode debug_mode, int *ierr);
#else
void adios2_init_nompi_(adios2_ADIOS **adios, const int debug_mode, int *ierr);

void adios2_init_config_nompi_(adios2_ADIOS **adios, const char *config_file,
                               const int debug_mode, int *ierr);
#endif

void adios2_declare_io_(adios2_IO **io, adios2_ADIOS **adios,
                        const char *io_name, int *ierr);

#ifdef __cplusplus
}
#endif

#endif /* BINDINGS_FORTRAN_ADIOS2_F_H_ */
