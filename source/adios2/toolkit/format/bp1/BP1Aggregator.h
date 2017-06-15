/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BP1Aggregator.h
 *
 *  Created on: Mar 1, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BP1_BP1AGGREGATOR_H_
#define ADIOS2_TOOLKIT_FORMAT_BP1_BP1AGGREGATOR_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <string>
/// \endcond

#include "adios2/ADIOSConfig.h"
#include "adios2/ADIOSMPICommOnly.h"
#include "adios2/toolkit/capsule/heap/STLVector.h"
#include "adios2/toolkit/format/bp1/BP1Structs.h"

namespace adios
{
namespace format
{

/** Does all MPI aggregation tasks */
class BP1Aggregator
{

public:
    MPI_Comm m_MPIComm;  ///< MPI communicator from Engine
    int m_RankMPI = 0;   ///< current MPI rank process
    int m_SizeMPI = 1;   ///< current MPI processes size
    int m_Processes = 1; ///< number of aggregated MPI processes

    /**
     * Unique constructor
     * @param mpiComm coming from engine
     */
    BP1Aggregator(MPI_Comm mpiComm, const bool debugMode = false);

    ~BP1Aggregator() = default;

    /**
     * Function that aggregates and writes (from rank = 0) profiling.log in
     * python dictionary format
     * @param rankLog contain rank profiling info to be aggregated
     */
    std::string GetGlobalProfilingLog(const std::string &rankLog) const;

    void SetCollectiveBP1Index(const BP1MetadataSet &metadataSet,
                               capsule::STLVector &heapBuffer) const;

private:
    const bool m_DebugMode = false;

    /**
     * Checks an MPI received size using size_t
     * @param size to be checked
     * @param rankSource used to improve exception message
     * @param rankDestination used to improve exception message
     * @param hint used to improve exception message
     */
    void CheckReceivedSize(const size_t size, const int rankSource,
                           const int rankDestination,
                           const std::string hint) const;

    /**
     * Gather indices from all sender ranks to rank zero (not included).
     * @param indexName used for debugging purposes
     * @param bpIndex sender: sends BPIndex1.Buffer and BPIndex1.Offsets
     * @param buffers receiver: from BPIndex1.Buffer from every sender
     * @param offsets receiver: from BPIndex1.Offsets from every sender
     */
    void GatherBP1Index(const std::string indexName, const BP1Index &bpIndex,
                        std::vector<std::vector<char>> &buffers,
                        std::vector<std::vector<size_t>> &offsets) const;

    /**
     * Only used by rank 0, sorts and flattens a single bp1Index
     * @param indexName  used for debugging purposes
     * @param bpIndex local rank0 bpIndex
     * @param buffers MPI received bpIndex buffers
     * @param offsets MPI received bpIndex offsets
     * @param heapBuffer
     */
    void FlattenBP1Index(const std::string indexName, const BP1Index &bpIndex,
                         std::vector<std::vector<char>> &buffers,
                         std::vector<std::vector<size_t>> &offsets,
                         capsule::STLVector &heapBuffer) const;
};

} // end namespace format
} // end namespace adios

#endif /* ADIOS2_UTILITIES_FORMAT_BP1_BP1AGGREGATOR_H_ */
