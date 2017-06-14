/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPFileWriter.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BPFileWriter.h"
#include "BPFileWriter.tcc"

#include <iostream>

#include "adios2/ADIOSMPI.h"
#include "adios2/core/IO.h"
#include "adios2/helper/adiosFunctions.h" //CheckIndexRange
#include "adios2/toolkit/transport/file/FileDescriptor.h"
#include "adios2/toolkit/transport/file/FilePointer.h"
#include "adios2/toolkit/transport/file/FileStream.h"

namespace adios
{

BPFileWriter::BPFileWriter(IO &io, const std::string &name,
                           const OpenMode openMode, MPI_Comm mpiComm)
: Engine("BPFileWriter", io, name, openMode, mpiComm),
  m_BP1Writer(mpiComm, m_DebugMode), m_TransportsManager(mpiComm, m_DebugMode)
{
    m_EndMessage = " in call to IO Open BPFileWriter " + m_Name + "\n";
    Init();
}

BPFileWriter::~BPFileWriter() = default;

void BPFileWriter::Init()
{
    InitParameters();
    InitTransports();
    InitBPBuffer();
}

#define declare_type(T)                                                        \
    void BPFileWriter::DoWrite(Variable<T> &variable, const T *values)         \
    {                                                                          \
        DoWriteCommon(variable, values);                                       \
    }
ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type

void BPFileWriter::Advance(const float /*timeout_sec*/)
{
    m_BP1Writer.Advance();
}

void BPFileWriter::Close(const int transportIndex)
{
    if (m_DebugMode)
    {
        m_TransportsManager.CheckTransportIndex(transportIndex);
    }

    // close bp buffer by flattening data and metadata
    m_BP1Writer.FlattenBuffer();
    // send flattened buffer data to corresponding transports
    m_TransportsManager.WriteFiles(m_BP1Writer.m_HeapBuffer.GetData(),
                                   m_BP1Writer.m_HeapBuffer.m_DataPosition,
                                   transportIndex);

    m_TransportsManager.CloseFiles(transportIndex);

    if (m_BP1Writer.m_Profiler.IsActive &&
        m_TransportsManager.AllTransportsClosed())
    {
        CloseProfiling();
    }

    if (m_TransportsManager.m_GetCollectiveMetadata)
    {
        CloseCollectiveMetadata();
    }
}

// PRIVATE FUNCTIONS
void BPFileWriter::InitParameters()
{
    m_BP1Writer.InitParameters(m_IO.m_Parameters);
}

void BPFileWriter::InitTransports()
{
    // TODO need to add support for aggregators here later
    if (m_IO.m_TransportsParameters.empty())
    {
        Params defaultTransportParameters;
        defaultTransportParameters["transport"] = "File";
        m_IO.m_TransportsParameters.push_back(defaultTransportParameters);
    }

    // Set additional parameters

    // Form bp names
    auto transportsNames = m_TransportsManager.GetFilesBaseNames(
        m_Name, m_IO.m_TransportsParameters);
    auto bpBaseNames = m_BP1Writer.GetBPBaseNames(transportsNames);
    auto bpNames = m_BP1Writer.GetBPNames(transportsNames);

    m_TransportsManager.OpenFiles(bpBaseNames, bpNames, m_OpenMode,
                                  m_IO.m_TransportsParameters,
                                  m_BP1Writer.m_Profiler.IsActive);
}

void BPFileWriter::InitBPBuffer()
{
    if (m_OpenMode == OpenMode::Append)
    {
        throw std::invalid_argument(
            "ADIOS2: OpenMode Append hasn't been implemented, yet");
        // TODO: Get last pg timestep and update timestep counter in
    }
    else
    {
        m_BP1Writer.WriteProcessGroupIndex(
            m_IO.m_HostLanguage, m_TransportsManager.GetTransportsTypes());
    }
}

void BPFileWriter::CloseProfiling() noexcept
{
    auto transportTypes = m_TransportsManager.GetTransportsTypes();
    auto transportProfilers = m_TransportsManager.GetTransportsProfilers();

    const std::string log(
        m_BP1Writer.GetRankProfilingLog(transportTypes, transportProfilers));

    const std::string profilingLog(m_BP1Writer.AggregateProfilingLog(log));

    if (m_BP1Writer.m_BP1Aggregator.m_RankMPI == 0)
    {
        transport::FileStream profilingLogStream(m_MPIComm, m_DebugMode);
        auto bpBaseNames = m_BP1Writer.GetBPBaseNames({m_Name});
        profilingLogStream.Open(bpBaseNames[0] + "/profiling.log",
                                OpenMode::Write);
        profilingLogStream.Write(profilingLog.c_str(), profilingLog.size());
        profilingLogStream.Close();
    }
}

/** Called from Close, adds global metadata to transports */
void BPFileWriter::CloseCollectiveMetadata() noexcept
{
    m_BP1Writer.SetCollectiveMetadata();
    // send Collective Metadata to transports
}

} // end namespace adios
