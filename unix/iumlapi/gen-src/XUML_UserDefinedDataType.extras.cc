//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
void iUMLAPI::XUML::UserDefinedDataType::setDataTypeName( const std::string& name )
{
  iUMLAPI::edit_data_type(entity.getEntity(),name, Unchanged);
}


