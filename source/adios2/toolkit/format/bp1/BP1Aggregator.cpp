/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BP1Aggregator.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BP1Aggregator.h"

/// \cond EXCLUDE_FROM_DOXYGEN
#include <fstream>
#include <ios>
#include <stdexcept>
#include <vector>
/// \endcond

#include <iostream>

#include "adios2/ADIOSMPI.h"

namespace adios
{
namespace format
{

BP1Aggregator::BP1Aggregator(MPI_Comm mpiComm, const bool debugMode)
: m_MPIComm(mpiComm), m_DebugMode(debugMode)
{
    MPI_Comm_rank(m_MPIComm, &m_RankMPI);
    MPI_Comm_size(m_MPIComm, &m_SizeMPI);
}

std::string
BP1Aggregator::GetGlobalProfilingLog(const std::string &rankLog) const
{
    std::string profilingLog;

    if (m_RankMPI == 0)
    {
        const unsigned int sizeMPI = static_cast<const unsigned int>(m_SizeMPI);
        std::vector<std::vector<char>> rankLogs(sizeMPI - 1); // other ranks
        std::vector<int> rankLogsSizes(sizeMPI - 1, -1);      // init with -1
        std::vector<MPI_Request> requests(sizeMPI);
        std::vector<MPI_Status> statuses(sizeMPI);

        // first receive sizes
        for (unsigned int i = 1; i < sizeMPI; ++i)
        {
            MPI_Irecv(&rankLogsSizes[i - 1], 1, MPI_INT, i, 0, m_MPIComm,
                      &requests[i]);
        }

        // wait for each size and allocate
        for (unsigned int i = 1; i < sizeMPI; ++i)
        {
            MPI_Wait(&requests[i], &statuses[i]);
            if (m_DebugMode)
            {
                if (rankLogsSizes[i - 1] == -1)
                    throw std::runtime_error(
                        "ERROR: couldn't get size from rank " +
                        std::to_string(i) +
                        ", in ADIOS aggregator for Profiling.log\n");
            }
            rankLogs[i - 1].resize(rankLogsSizes[i - 1]);
        }

        // receive rankLog from other ranks
        for (unsigned int i = 1; i < sizeMPI; ++i)
        {
            MPI_Irecv(rankLogs[i - 1].data(), rankLogsSizes[i - 1], MPI_CHAR, i,
                      1, m_MPIComm, &requests[i]);
        }

        // wait for all messages to arrive
        for (unsigned int i = 1; i < sizeMPI; ++i)
        {
            MPI_Wait(&requests[i], &statuses[i]);
            // needs runtime exception checks
        }

        // write global string
        // key is to reserve memory first
        profilingLog.reserve(rankLog.size() * m_SizeMPI);

        profilingLog += "{\n";
        profilingLog += rankLog + ",\n";
        for (unsigned int i = 1; i < sizeMPI; ++i)
        {
            const std::string rankLogStr(rankLogs[i - 1].data(),
                                         rankLogs[i - 1].size());
            profilingLog += rankLogStr + ",";
            if (i < sizeMPI - 1)
            {
                profilingLog += "\n";
            }
        }
        profilingLog.pop_back(); // eliminate trailing comma
        profilingLog += "\n";
        profilingLog += "}\n";
    }
    else
    {
        const int rankLogSize = static_cast<const int>(rankLog.size());
        MPI_Request requestSize;
        MPI_Isend(&rankLogSize, 1, MPI_INT, 0, 0, m_MPIComm, &requestSize);

        MPI_Request requestRankLog;
        MPI_Isend(const_cast<char *>(rankLog.c_str()), rankLogSize, MPI_CHAR, 0,
                  1, m_MPIComm, &requestRankLog);
    }

    MPI_Barrier(m_MPIComm); // Barrier here?

    return profilingLog;
}

void BP1Aggregator::SetCollectivePGIndex(const BP1Index &pgIndex,
                                         capsule::STLVector &heapBuffer) const
{
    if (m_RankMPI == 0)
    {
        // first send total buffer sizes
        const unsigned int sizeMPI = static_cast<const unsigned int>(m_SizeMPI);
        const unsigned int senders = sizeMPI - 1;

        // Pair of size_t */
        std::vector<size_t> pgIndexSizes(2 * senders, UndefinedSize);

        std::vector<std::vector<char>> pgBuffers(senders);
        std::vector<std::vector<size_t>> pgOffsets(senders);

        // half for index Buffer, half for index Offsets
        std::vector<MPI_Request> requests(senders);
        std::vector<MPI_Status> statuses(senders);

        // first receive sizes of indices and offsets
        for (unsigned int s = 0; s < senders; ++s)
        {
            MPI_Irecv(&pgIndexSizes[2 * s], 2, ADIOS2_MPI_SIZE_T, s + 1, 0,
                      m_MPIComm, &requests[s]);
        }

        // wait for each size and allocate
        for (unsigned int s = 0; s < senders; ++s)
        {
            MPI_Wait(&requests[s], &statuses[s]);
            const size_t bufferSize = pgIndexSizes[2 * s];
            const size_t offsetsSize = pgIndexSizes[2 * s + 1];
            if (m_DebugMode)
            {
                CheckSize(bufferSize, s + 1, 0, "for pgBuffer");
                CheckSize(offsetsSize, s + 1, 0, "for pgOffset");
            }
            pgBuffers[s].resize(bufferSize);
            pgOffsets[s].resize(offsetsSize);
        }
    }
    else
    {
        const std::vector<size_t> pgSizes = {pgIndex.Buffer.size(),
                                             pgIndex.Offsets.size()};
        MPI_Request sizesRequest;
        MPI_Isend(pgSizes.data(), 2, ADIOS2_MPI_SIZE_T, 0, 1, m_MPIComm,
                  &sizesRequest);
    }
}

// PRIVATE
void BP1Aggregator::CheckSize(const size_t size, const int rankSource,
                              const int rankDestination,
                              const std::string hint) const
{
    if (size == UndefinedSize)
    {
        throw std::runtime_error(
            "ERROR: BP1 Aggregator rank " + std::to_string(rankDestination) +
            " couldn't receive size from rank " + std::to_string(rankSource) +
            " " + hint + ",  in call to Close \n");
    }
}

} // end namespace format
} // end namespace adios
