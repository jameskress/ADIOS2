/*
 * VisVTKm.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: wfg
 */

#include "VisVTKm.h"

#ifndef VTKM_DEVICE_ADAPTER
#define VTKM_DEVICE_ADAPTER VTKM_DEVICE_ADAPTER_SERIAL
#endif

#include <iostream>
#include <vtkm/Math.h>
#include <vtkm/cont/DataSetBuilderUniform.h>
#include <vtkm/cont/DataSet.h>

namespace adios2
{

bool VisVTKm::RenderAllVariables(IO &io)
{
    for (auto &variableName : m_VariablesNames)
    {
        auto *var1 = io.GetVariableBase("variableName");
        std::cout<<var1->m_Shape.size()<<" : "<<var1->m_Shape[0]<<std::endl;

        for( auto& transform : var1->m_TransformsInfo )
        {
            std::cout<<__LINE__<<std::endl;
            for(auto& parameter : transform.Parameters)
            {
                std::cout<<__LINE__<<std::endl;
                std::cout<<parameter.first<<std::endl;
                if( parameter.first == "X1" )
                {
                    auto value = parameter.second;
                    std::cout<<__LINE__<<std::endl;
                    std::cout<<"Meow"<<std::endl;
                    ///CAll VTKm magic
                }
                
            }
        }
    }

    vtkm::Id3 dims(10,10,10);
    vtkm::cont::DataSetBuilderUniform dsb;
    vtkm::cont::DataSet ds = dsb.Create(dims);
    ds.PrintSummary(std::cout);
    std::cout<<dims<<std::endl;

    return true;
}
}
