/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Transform.h : Base class for all transforms under adios2/transform
 *
 *  Created on: Oct 17, 2016
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_CORE_TRANSFORM_H_
#define ADIOS2_CORE_TRANSFORM_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <string>
#include <vector>
/// \endcond

#include "adios2/ADIOSConfig.h"

namespace adios2
{

/** Base class that defines data variable transformations */
class Transform
{

public:
    /** From derived class to identify the transform */
    const std::string m_Method;

    /**
     * Initialize parent method
     * @param method zlib, bzip2, szip
     */
    Transform(const std::string method);

    virtual ~Transform() = default;

    virtual void Compress(const std::vector<char> &bufferIn,
                          std::vector<char> &bufferOut);

    virtual void Decompress(const std::vector<char> &bufferIn,
                            std::vector<char> &bufferOut);
};

} // end namespace adios
#endif /* ADIOS2_CORE_TRANSFORM_H_ */
