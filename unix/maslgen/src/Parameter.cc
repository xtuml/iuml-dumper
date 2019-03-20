// 
// Filename : Parameter.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Parameter.hh"

#include "TextUtils.hh"
#include "DataType.hh"

#include "iumlapi/XUML_DataType.hh"
#include "iumlapi/XUML_Attribute.hh"

MaslGen::Parameter::Parameter ( const iUMLAPI::XUML::OperationParameter& param )
  : name(makeIdentifier(param.getName())),
    type((param.getIsASet()?"sequence of ":"") + DataType(param.getHasTypeDataTypeR207().isValid()
                             ? param.getHasTypeDataTypeR207()
                             :  param.getHasNameAndOrTypeLinkedToAttributeR203().getHasItsDomainSpecifiedByDataTypeR71()).getName())
{
}


MaslGen::Parameter::Parameter ( const iUMLAPI::XUML::SignalParameter& param )
  : name(makeIdentifier(param.getParameterName())),
    type((param.getIsASet()?"sequence of ":"") + DataType(param.getHasTypeDataTypeR191().isValid()
                             ? param.getHasTypeDataTypeR191()
                             :  param.getHasNameAndOrTypeLinkedToAttributeR192().getHasItsDomainSpecifiedByDataTypeR71()).getName())
{
}
