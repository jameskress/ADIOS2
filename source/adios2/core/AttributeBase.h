/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * AttributeBase.h
 *
 *  Created on: Jun 12, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

/// \cond EXCLUDE_FROM_DOXYGEN
#include <memory> //std::shared_ptr
#include <string>
#include <unordered_map>
/// \endcond

namespace adios
{

class AttributeBase
{

public:
    const std::string m_Name;
    const std::string m_Type;

    /**
     * Only primitive types are accepted
     * @param type input type
     * */
    AttributeBase(const std::string &name, const std::string type);

    virtual ~AttributeBase() = default;
};

using Attributes =
    std::unordered_map<std::string, std::shared_ptr<AttributeBase>>;

} // end namespace adios
