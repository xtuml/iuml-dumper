//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/XUML_OperationParameter.hh"

iUMLAPI::XUML::OperationParameter iUMLAPI::XUML::NonCounterpartTerminatorOperation::createParameter ( const std::string& name_, bool out )
{  
  return createOperationParameter(name_,out?outParam:inParam);
}

