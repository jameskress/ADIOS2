/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Attribute.h template classes extending AttributeBase
 *
 *  Created on: Jun 9, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_CORE_ATTRIBUTE_H_
#define ADIOS2_CORE_ATTRIBUTE_H_

namespace adios
{

/** @brief Attributes provide complementary information to IO Variables*/
template <class T>
class Attribute : public AttributeBase
{

public:
    const T *m_ArrayData = nullptr;
    const size_t m_Elements;
    const bool m_IsArray = false;

    const std::vector<T> *m_Data = nullptr;

    /**
     * Constructor from array data, used to avoid memory duplication from C
     * @param name
     * @param data
     * @param elements
     */
    Attribute(const std::string &name, const T *arrayData,
              const size_t elements);

    Attribute(const std::string &name, const std::vector<T> &data);

    ~Attribute() = default;
};

} // end namespace adios

#endif /* ADIOS2_CORE_ATTRIBUTE_H_ */
