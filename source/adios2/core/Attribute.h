/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Attribute.h
 *
 *  Created on: Jun 9, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_CORE_ATTRIBUTE_H_
#define ADIOS2_CORE_ATTRIBUTE_H_

namespace adios
{

/** @brief Complementary information to IO */
struct Attribute
{
    /** only primitive types */
    const std::string Type;
    /** data as string to be casted to the right type */
    const std::string Data;
};

} // end namespace adios

#endif /* ADIOS2_CORE_ATTRIBUTE_H_ */
