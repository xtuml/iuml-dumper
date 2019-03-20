// 
// Filename : iUMLAPI.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef iUMLAPI_HH
#define iUMLAPI_HH

#include <api/api.h>

#include <string>
#include <exception>

namespace iUMLAPI
{
  void initialise();

  void logon ( const std::string& name, API_ENTITY& db );
  void logoff();

  void read_relationship ( API_ENTITY start, const std::string& relationship, API_ENTITY_LIST*& result );
  void release_entity_list ( API_ENTITY_LIST*& entity_list );
  void read_attribute ( API_ENTITY entity, const std::string& name, API_ATTRIBUTE& value );
  void release_attribute ( API_ATTRIBUTE& value );
  void set_domain ( const std::string& domain );
  void copy_of_entity ( API_ENTITY src, API_ENTITY& dest );
  bool same_entity ( API_ENTITY lhs, API_ENTITY rhs );
  void release_entity ( API_ENTITY entity );

  extern "C"
  {
    typedef API_STATUS (*APIFunction) (API_ENTITY_LIST*,API_ATTRIBUTE_LIST*,API_ATTRIBUTE_LIST**);
  }

  class AttributeProxy
  {
    public:
      AttributeProxy ( API_ATTRIBUTE value );
      AttributeProxy ( const std::string& att );
      AttributeProxy ( const char* const att );
      AttributeProxy ( long att );
      AttributeProxy ( int att );
      AttributeProxy ( bool att );
      AttributeProxy ( const AttributeProxy& att1, const AttributeProxy& att2 );
      AttributeProxy ( const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3 );

      operator API_ATTRIBUTE*() const;

    private:
      API_ATTRIBUTE* result;
  };

  class EntityProxy
  {
    public:
      EntityProxy ( API_ENTITY value );
      operator API_ENTITY*();

    private:
      API_ENTITY result;
  };

  void api_delete ( APIFunction apiFn, EntityProxy e1 );
  void api_create ( APIFunction apiFn, EntityProxy e1, API_ENTITY& outEntity );
  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att, API_ENTITY& outEntity );
  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, API_ENTITY& outEntity );
  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, API_ENTITY& outEntity );
  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, const AttributeProxy& att4, API_ENTITY& outEntity );
  void api_create ( APIFunction apiFn, EntityProxy e1, const AttributeProxy& att1, const AttributeProxy& att2, const AttributeProxy& att3, const AttributeProxy& att4, const AttributeProxy& att5, API_ENTITY& outEntity );


  void logon_update ( const std::string& name, API_ENTITY& db );
  void logoff_update();
  void logoff_abort_update();

  void acquire_top_level_lock( API_ENTITY src, API_ENTITY& dest );
  void acquire_analysis_area_lock( API_ENTITY src, API_ENTITY& dest );
  void acquire_project_version_lock( API_ENTITY src, API_ENTITY& dest );
  void update_attribute ( API_ENTITY entity, const std::string& name, API_ATTRIBUTE value );

  enum FreezeType { temporary = 1, permanent = 2, unfreeze = 3 };
  enum BaseType { Unchanged, Real, Integer, Text, Enumeration, Date, TimeOfDay, Deferred, DataStructure, ClassReference };
  enum InOutType { inParam = 1, outParam = 2 };
  enum TagType { unchanged, singleValue, enumeration };

  void freeze_domain_version ( API_ENTITY domainVersion, FreezeType freezeType );
  void freeze_project_version ( API_ENTITY projectVersion, FreezeType freezeType );
  void edit_data_type ( API_ENTITY dataType, const std::string& name, BaseType baseType );
  void edit_tag_group ( API_ENTITY tagGroup, const std::string& name );
  void edit_tag ( API_ENTITY tag, const std::string& name, TagType type );
  void edit_tag_enumeration ( API_ENTITY tagEnumeration, const std::string& value );
  void attach_tag ( API_ENTITY taggedItem, API_ENTITY tag, API_ENTITY& tagAttachment );
  void detach_tag ( API_ENTITY taggedItem, API_ENTITY tag );
  void define_tag_value ( API_ENTITY tagAttachment, const std::string& value );
  void edit_operation_parameter_type ( API_ENTITY parameter, const std::string& typeName, bool manyValued );

  class ErrorChecker
  {
    public:
      ErrorChecker();
      void operator() ( API_STATUS status );
    private:
      int lastRaised;
  };    

  class Exception : public std::exception
  {
    public:
     Exception ( API_STATUS status );
     Exception ( const std::string& functionName, const std::string& text );
     Exception ( const API_EXCEPTION_STRUCT& ex );  
     virtual const char* what() const throw();
     ~Exception() throw() {};

    private:
      long type;
      std::string functionName;  
      std::string text;
      static const std::string& iUMLAPI::Exception::lookupStatus ( API_STATUS status );
  };

  class Warning : public Exception
  {
    public:
      Warning ( const API_EXCEPTION_STRUCT& ex ) : Exception(ex) {}
      Warning ( API_STATUS status) 
        : Exception(status) {}
      Warning ( const std::string& functionName, const std::string& text ) 
        : Exception(functionName,text) {}
      ~Warning() throw() {};
  };

  class Error : public Exception
  {
    public:
      Error ( const API_EXCEPTION_STRUCT& ex ) : Exception(ex) {}
      Error ( API_STATUS status) 
        : Exception(status) {}
      Error ( const std::string& functionName, const std::string& text ) 
        : Exception(functionName,text) {}
      ~Error() throw() {};
  };
};
    
#endif
