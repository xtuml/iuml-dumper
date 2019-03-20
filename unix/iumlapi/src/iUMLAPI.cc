// 
// Filename : iUMLAPI.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/iUMLAPI.hh"
#include <map>
#include <iostream>
#include "boost/lexical_cast.hpp"


namespace iUMLAPI
{
  iUMLAPI::ErrorChecker checkExceptions;

  void checkResult ( API_ATTRIBUTE_LIST* result )
  {
    if ( !result->attribute->attr_value.v_bool )
    {
      throw Error(API_FAILURE);
    }
  }

  AttributeProxy::AttributeProxy ( API_ATTRIBUTE value )
  {
    API_copy_of_attribute ( &value, &result );
  }

  AttributeProxy:: AttributeProxy ( const std::string& att )
  {
    create_api_attribute ( &result, API_STRING, att.c_str());
  }

  AttributeProxy::AttributeProxy ( const char* const att )
  {
    create_api_attribute ( &result, API_STRING, att);
  }

  AttributeProxy::AttributeProxy ( long att )
  {
    create_api_attribute ( &result, API_INTEGER, att);
  }

  AttributeProxy::AttributeProxy ( int att )
  {
    create_api_attribute ( &result, API_INTEGER, static_cast<long>(att));
  }

  AttributeProxy::AttributeProxy ( bool att )
  {
    create_api_attribute ( &result, API_BOOLEAN, att);
  }

  AttributeProxy::AttributeProxy ( const AttributeProxy& att1, const AttributeProxy& att2 )
  {
    API_ATTRIBUTE_LIST* list = 0;
    add_attribute_to_list(att1,&list);
    add_attribute_to_list(att2,&list);

    create_api_attribute ( &result, API_ATTR_LIST_TYPE, list);
  }

  AttributeProxy::AttributeProxy ( const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3 )
  {
    API_ATTRIBUTE_LIST* list = 0;
    add_attribute_to_list(att1,&list);
    add_attribute_to_list(att2,&list);
    add_attribute_to_list(att3,&list);

    create_api_attribute ( &result, API_ATTR_LIST_TYPE, list);
  }

  AttributeProxy::operator API_ATTRIBUTE*() const
  {
    return result;
  }


  EntityProxy::EntityProxy ( API_ENTITY value )
  {
    copy_of_entity ( value, result );
  }

  EntityProxy::operator API_ENTITY*()
  {
    return &result;
  }


  void api_transform ( APIFunction apiFn, EntityProxy e1, API_ENTITY& outEntity )
  {
    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,0,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_create ( APIFunction apiFn, EntityProxy e1, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);
    add_attribute_to_list(att3,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }


  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, const AttributeProxy& att4, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);
    add_attribute_to_list(att3,&params);
    add_attribute_to_list(att4,&params);


    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }


  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, const AttributeProxy& att4, const AttributeProxy& att5, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);
    add_attribute_to_list(att3,&params);
    add_attribute_to_list(att4,&params);
    add_attribute_to_list(att5,&params);


    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }



  void api_edit ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }


  void api_edit ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2  )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_edit ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3 )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);
    add_attribute_to_list(att3,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }



  void api_edit ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, const AttributeProxy& att4 )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* params = 0;
    add_attribute_to_list(att1,&params);
    add_attribute_to_list(att2,&params);
    add_attribute_to_list(att3,&params);
    add_attribute_to_list(att4,&params);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,params,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&params));  
    checkExceptions(API_release_attribute_list(&result));  

  }




  void api_delete ( APIFunction apiFn, EntityProxy e1 )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);

    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,0,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&result));  

  }


  void api_link ( APIFunction apiFn, EntityProxy e1, EntityProxy e2, API_ENTITY& outEntity )
  {

    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);
    add_entity_to_list(e2,&entityList);


    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,0,&result));
    checkResult(result);

    copy_of_entity(*result->next_attribute->attribute->attr_value.v_entity,outEntity);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&result));  

  }

  void api_unlink ( APIFunction apiFn, EntityProxy e1, EntityProxy e2 )
  {
    API_ENTITY_LIST* entityList = 0;
    add_entity_to_list(e1,&entityList);
    add_entity_to_list(e2,&entityList);


    API_ATTRIBUTE_LIST* result = 0;
    checkExceptions(apiFn(entityList,0,&result));
    checkResult(result);

    checkExceptions(API_release_entity_list(&entityList));  
    checkExceptions(API_release_attribute_list(&result));  

  }


}




void iUMLAPI::initialise()
{
  static bool initialised = false;
  if ( !initialised )
  {
    checkExceptions(API_initialise());
    initialised = true;
  }
}

void iUMLAPI::logon ( const std::string& name, API_ENTITY& db )
{
  initialise();
  checkExceptions(API_logon(const_cast<char*>(name.c_str()),&db));
}

void iUMLAPI::logon_update ( const std::string& name, API_ENTITY& db )
{
  initialise();
  checkExceptions(API_logon_update(const_cast<char*>(name.c_str()),&db));
}

void iUMLAPI::logoff()
{
  API_logoff();
}

void iUMLAPI::logoff_update()
{
  API_logoff_update();
}

void iUMLAPI::logoff_abort_update()
{
  API_logoff_abort_update();
}

void iUMLAPI::read_relationship ( API_ENTITY start, const std::string& relationship, API_ENTITY_LIST*& result )
{
  checkExceptions(API_read_relationship(start,const_cast<char*>(relationship.c_str()),&result));
}

void iUMLAPI::release_entity_list ( API_ENTITY_LIST*& entity_list )
{
  checkExceptions(API_release_entity_list(&entity_list));
}

void iUMLAPI::read_attribute ( API_ENTITY entity, const std::string& name, API_ATTRIBUTE& value )
{
  checkExceptions(API_read_attribute(entity,const_cast<char*>(name.c_str()),&value));
}

void iUMLAPI::release_attribute ( API_ATTRIBUTE& value )
{
  checkExceptions(API_release_attribute(&value));
}

void iUMLAPI::set_domain ( const std::string& domain )
{
  checkExceptions(API_set_domain(const_cast<char*>(domain.c_str())));
}

void iUMLAPI::copy_of_entity ( API_ENTITY src, API_ENTITY& dest )
{
  checkExceptions(API_copy_of_entity(src,&dest));
}

bool iUMLAPI::same_entity ( API_ENTITY lhs, API_ENTITY rhs )
{
  return API_same_entity(lhs,rhs);
}

void iUMLAPI::release_entity ( API_ENTITY entity )
{
  checkExceptions(API_release_entity(entity));
}

void iUMLAPI::acquire_top_level_lock ( API_ENTITY src, API_ENTITY& dest )
{
  api_transform ( &API_acquire_top_level_lock, src, dest );
}

void iUMLAPI::acquire_analysis_area_lock ( API_ENTITY src, API_ENTITY& dest )
{
  api_transform ( &API_acquire_analysis_area_lock, src, dest );
}

void iUMLAPI::acquire_project_version_lock ( API_ENTITY src, API_ENTITY& dest )
{
  api_transform ( &API_acquire_project_version_lock, src, dest );
}

void iUMLAPI::update_attribute ( API_ENTITY entity, const std::string& name, API_ATTRIBUTE value )
{
  api_edit ( &API_update_attribute, entity, name, value );
}

void iUMLAPI::freeze_project_version ( API_ENTITY projectVersion, FreezeType freezeType )
{
  api_edit ( &API_freeze_project_version, projectVersion, static_cast<long>(freezeType) );
}

void iUMLAPI::freeze_domain_version ( API_ENTITY domainVersion, FreezeType freezeType )
{
  api_edit ( &API_freeze_domain_version, domainVersion, static_cast<long>(freezeType) );
}

void iUMLAPI::edit_data_type ( API_ENTITY dataType, const std::string& name, BaseType baseType )
{
  api_edit ( &API_edit_data_type, dataType, name, baseType );
}

void iUMLAPI::edit_tag_group ( API_ENTITY tagGroup, const std::string& name )
{
  api_edit ( &API_edit_tag_group, tagGroup, name );
}

void iUMLAPI::edit_tag ( API_ENTITY tag, const std::string& name, TagType type )
{
  api_edit ( &API_edit_tag, tag, name, static_cast<long>(type) );
}

void iUMLAPI::edit_tag_enumeration ( API_ENTITY tagEnumeration, const std::string& value )
{
  api_edit ( &API_edit_tag_enumeration, tagEnumeration, value );
}

void iUMLAPI::attach_tag ( API_ENTITY taggedItem, API_ENTITY tag, API_ENTITY& tagAttachment )
{
  api_link(&API_attach_tag,taggedItem,tag,tagAttachment);
}

void iUMLAPI::detach_tag ( API_ENTITY taggedItem, API_ENTITY tag )
{
  api_unlink(&API_detach_tag,taggedItem,tag);
}

void iUMLAPI::define_tag_value ( API_ENTITY tagAttachment, const std::string& value )
{
  api_edit ( &API_define_tag_value,tagAttachment, value );
}

void iUMLAPI::edit_operation_parameter_type ( API_ENTITY parameter, const std::string& typeName, bool manyValued )
{
  api_edit ( &API_edit_operation_parameter_type, parameter, 1, AttributeProxy(typeName,manyValued) );
}


iUMLAPI::ErrorChecker::ErrorChecker()
  : lastRaised(0)
{
}

void iUMLAPI::ErrorChecker::operator() ( API_STATUS status )
{
  int noExceptions;
  API_EXCEPTION_STRUCT* exceptions;
  API_get_exceptions(&noExceptions, &exceptions);

  if ( noExceptions > lastRaised )
  {
    if ( noExceptions > lastRaised + 1 )
    { 
      std::cerr << "Warning: " << noExceptions - lastRaised - 1 << " exeptions missed" << std::endl; 
    }

    if ( status == API_OK )
    { 
      std::cerr << "Warning: Exception Raised on OK status" << std::endl; 
    }

    lastRaised = noExceptions;
    if ( exceptions[noExceptions-1].exception_type == API_WARNING )
    {
      throw Warning(exceptions[noExceptions-1]);
    }
    else
    {
      throw Error(exceptions[noExceptions-1]);
    }
    
  }
  else
  {
    if ( status != API_OK )
    {
      throw Error(status);
    }
  }
}


iUMLAPI::Exception::Exception ( const API_EXCEPTION_STRUCT& ex )
  : type(ex.exception_type),
    functionName(ex.fn_name),
    text(ex.exception_text)
{
}


iUMLAPI::Exception::Exception ( API_STATUS status )
  : type(API_EXCEPTION),
    functionName("<unknown>"),
    text(lookupStatus(status))
{
}

iUMLAPI::Exception::Exception ( const std::string& functionName, const std::string& text )
  : type(API_EXCEPTION),
    functionName(functionName),
    text(text)
{
}

const std::string& iUMLAPI::Exception::lookupStatus ( API_STATUS status )
{
  static bool initialised = false;
  static std::map<API_STATUS,std::string> lookup;

  if ( !initialised )
  {
    initialised = true;
    lookup.insert(std::pair<API_STATUS,std::string>(API_FAILURE                , "Failure"                 ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_UNDEFINED              , "Undefined"               ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_FILE_NOT_FOUND         , "File not found"          ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NOT_LOGGED_ON          , "Not logged on"           ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NO_SUCH_REL            , "No such rel"             ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NO_SUCH_ENTITY         , "No such entity"          ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NO_SUCH_ATTRIBUTE      , "No such attribute"       ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NO_SUCH_DB             , "No such db"              ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_INDEX_OUT_OF_RANGE     , "Index out of range"      ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_INVALID_DOMAIN         , "Invalid domain"          ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_INDEX                  , "Index"                   ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NOT_IN_WRITE_MODE      , "Not in write mode"       ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_NOT_IN_READ_MODE       , "Not in read mode"        ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_ATTRIBUTE_NOT_WRITEABLE, "Attribute not writeable" ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_ATTRIBUTE_WRITE_FAILED , "Attribute write failed"  ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_INVALID_ATTRIBUTE_VALUE, "Invalid attribute value" ));
    lookup.insert(std::pair<API_STATUS,std::string>(API_PARTITION_LOCKED       , "Partition locked"        ));
  }

  return lookup[status];
}

const char* iUMLAPI::Exception::what() const throw()
{
  std::string result;
  switch ( type )
  {
    case API_EXCEPTION           : result = "Error ";               break;
    case API_NAMESPACE_VIOLATION : result = "Namespace Violation "; break;
    case API_INVALID_PARAMETER   : result = "Invalid Parameter ";   break;
    case API_INVALID_CALL        : result = "Invalid Call ";        break;
    case API_INTERNAL_ERROR      : result = "Internal Error ";      break;
    case API_WARNING             : result = "Warning ";             break;
    default                      : result = "Unrecognised Error ";  break;
  }

  result+= "in function " + functionName + ": " + text;
  return result.c_str();
}
