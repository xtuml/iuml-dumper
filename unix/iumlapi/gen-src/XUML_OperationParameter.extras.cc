//
// UK Crown Copyright (c) 2005. All Rights Reserved
//

void iUMLAPI::XUML::OperationParameter::setParameterType ( const std::string& typeName, bool manyValued )
{  
  reset();
  iUMLAPI::edit_operation_parameter_type(entity.getEntity(),typeName, manyValued);
}

