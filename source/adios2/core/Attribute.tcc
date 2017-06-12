/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Attribute.tcc
 *
 *  Created on: Jun 12, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_CORE_ATTRIBUTE_TCC_
#define ADIOS2_CORE_ATTRIBUTE_TCC_

#include "Attribute.h"

#include "adios2/ADIOSMacros.h"
#include "adios2/helper/adiosFunctions.h"

namespace adios
{

#define declare_type(T)                                                        \
                                                                               \
    template <>                                                                \
    Attribute<T>::Attribute(const std::string &name, const T *arrayData,       \
                            const size_t elements)                             \
    : AttributeBase(name, GetType<T>()), m_ArrayData(arrayData),               \
      m_Elements(elements), m_IsArray(true)                                    \
    {                                                                          \
    }                                                                          \
                                                                               \
    template <>                                                                \
    Attribute<T>::Attribute(const std::string &name,                           \
                            const std::vector<T> &data)                        \
    : AttributeBase(name, GetType<T>()), m_Data(&data)                         \
    {                                                                          \
    }

ADIOS2_FOREACH_PRIMITIVE_TYPE_1ARG(declare_type)
#undef declare_type

} // end namespace adios
