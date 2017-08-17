/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPWriter.cpp: Simple self-descriptive example of how to write a variable
 * to a BP File that lives in several MPI processes.
 *
 *  Created on: Feb 16, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <ios>      //std::ios_base::failure
#include <iostream> //std::cout
#include <mpi.h>
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /** Application variable */
    static const std::size_t Nx=10, Ny=10, Nz=10;
    std::vector<float> myTemperature(Nx*Ny*Nz);
    for (int i = 0; i < 10*10*10; i++)
        myTemperature[i] = i;

    try
    {
        /** ADIOS class factory of IO class objects, DebugON is recommended */
        adios2::ADIOS adios(MPI_COMM_WORLD, adios2::DebugON);

        auto &visTransform = adios.GetTransform("Vis", {{"iso", "3.1"}});

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO &bpIO = adios.DeclareIO("VisTemperature");

        bpIO.SetParameters({{"Vis", "OFF"}});

        /** global array : name, { shape (total) }, { start (local) }, { count
         * (local) }, all are constant dimensions */
        adios2::Variable<float> &bpTemperature =
            bpIO.DefineVariable<float>("variableName", {size*Nx,Ny,Nz},{rank*Nx,0,0}, {Nx,Ny,Nz},
                                        adios2::ConstantDims);

        bpTemperature.AddTransform(visTransform);

        /** Engine derived class, spawned to start IO operations */
        auto bpWriter = bpIO.Open("vis.bp", adios2::OpenMode::Write);

        if (!bpWriter)
        {
            throw std::ios_base::failure(
                "ERROR: bpWriter not created at Open\n");
        }

        /** Write variable for buffering */
        bpWriter->Write<float>(bpTemperature, myTemperature.data());

        bpWriter->Advance();

        /** Create bp file, engine becomes unreachable after this*/
        bpWriter->Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM from rank "
                  << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout
            << "IO System base failure exception, STOPPING PROGRAM from rank "
            << rank << "\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM from rank " << rank << "\n";
        std::cout << e.what() << "\n";
    }

    MPI_Finalize();

    return 0;
}