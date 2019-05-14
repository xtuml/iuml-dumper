// 
// Filename : Entity.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/iUMLAPI.hh"
#include "iumlapi/Entity.hh"
#include <algorithm>
#include <iterator>
#include <iostream>

iUMLAPI::Entity::Entity() 
  : valid(false)
{
}

iUMLAPI::Entity::Entity(const API_ENTITY& other)
  : valid(true)
{
  iUMLAPI::copy_of_entity(other,entity);
}


iUMLAPI::Entity::Entity(const Entity& other)
{
  if ( other.valid )
  {
    iUMLAPI::copy_of_entity(other.entity,entity);
    valid = true;
  }
  else
  {
    valid = false;
  }

}

iUMLAPI::Entity& iUMLAPI::Entity::operator= ( const API_ENTITY& other )
{
  if ( valid && ! iUMLAPI::same_entity(other,entity) )
  {
    iUMLAPI::release_entity(entity);
  }
  iUMLAPI::copy_of_entity(other,entity);
  valid = true;

  return *this;
}

iUMLAPI::Entity& iUMLAPI::Entity::operator= ( const Entity& other )
{
  if ( other.valid )
  {
    if ( valid && ! iUMLAPI::same_entity(other.entity,entity) )
    {
      iUMLAPI::release_entity(entity);
    }
    iUMLAPI::copy_of_entity(other.entity,entity);
    valid = true;
  }
  else
  {
    if ( valid )
    {
      iUMLAPI::release_entity(entity);
      valid = false;
    }
  }
  return *this;
}

iUMLAPI::Entity::~Entity()
{
  if ( valid )
  {
    try
    {
      iUMLAPI::release_entity(entity);
    }
    catch(...)
    {
      std::cerr << "Exception thrown in release." << std::endl;
    }
  }
}

API_ATTR_TYPE iUMLAPI::Entity::getAttributeType ( const std::string& name, UMLDomain domain ) const
{
  setUMLDomain(domain);
  if ( !valid )
  {
    throw iUMLAPI::Error("getAttributeType","Invalid entity");
  }
  API_ATTRIBUTE value;
  iUMLAPI::read_attribute(entity,name,value);
  API_ATTR_TYPE result = value.attr_type;
  iUMLAPI::release_attribute(value);
  return result;
}

iUMLAPI::Entity::Attribute iUMLAPI::Entity::getAttribute ( const std::string& name, UMLDomain domain ) const
{
  setUMLDomain(domain);
  if ( !valid )
  {
    throw iUMLAPI::Error("getAttribute","Invalid entity");
  }
  API_ATTRIBUTE value;
  iUMLAPI::read_attribute(entity,name,value);
  Attribute result(value);
  iUMLAPI::release_attribute(value);
  return result;
}

void iUMLAPI::Entity::setAttribute ( const std::string& name, const Attribute& value, UMLDomain domain )
{
  setUMLDomain(domain);
  if ( !valid )
  {
    throw iUMLAPI::Error("setAttribute","Invalid entity");
  }
  iUMLAPI::update_attribute(entity,name,value);
}

std::vector<iUMLAPI::Entity> iUMLAPI::Entity::navigate ( const std::string& relationship, UMLDomain domain ) const
{
  if (valid)
  {
    std::string::size_type commaPos = relationship.find(',');
    std::string firstHop = (commaPos==std::string::npos)?relationship:relationship.substr(0,commaPos);
    std::string toNavigate = (commaPos==std::string::npos)?"":relationship.substr(commaPos+1);

    setUMLDomain(domain);
    API_ENTITY_LIST* list = 0;
    iUMLAPI::read_relationship(entity,firstHop,list);
    std::vector<Entity> result;
    API_ENTITY_LIST* element = list;
    while ( element )
    {
      if ( toNavigate == "" )
      {
        result.push_back(element->entity);
      }
      else
      {
        std::vector<Entity> children = Entity(element->entity).navigate(toNavigate,domain);
        std::copy(children.begin(),children.end(),std::back_inserter(result));
      }
      element = element->next_entity;
    }

    if (list) iUMLAPI::release_entity_list(list);
    return result;
  }
  else
  {
    return std::vector<Entity>();
  }
}

iUMLAPI::Entity iUMLAPI::Entity::navigateOne ( const std::string& relationship, UMLDomain domain ) const
{
  if (valid)
  {
    setUMLDomain(domain);
    API_ENTITY_LIST* list = 0;
    iUMLAPI::read_relationship(entity,relationship,list);
    if ( list )
    {
      Entity result(list->entity);
      iUMLAPI::release_entity_list(list);
      return result;
    }
    else return Entity();  
  }
  else
  {
    return Entity();
  }
}

void iUMLAPI::Entity::acquireTopLevelLock()
{
  API_ENTITY editable;
  iUMLAPI::acquire_top_level_lock(entity,editable);
  iUMLAPI::release_entity(entity);
  entity = editable;
}

void iUMLAPI::Entity::acquireAnalysisAreaLock()
{
  API_ENTITY editable;
  iUMLAPI::acquire_analysis_area_lock(entity,editable);
  iUMLAPI::release_entity(entity);
  entity = editable;
}

void iUMLAPI::Entity::acquireProjectVersionLock()
{
  API_ENTITY editable;
  iUMLAPI::acquire_project_version_lock(entity,editable);
  iUMLAPI::release_entity(entity);
  entity = editable;
}

iUMLAPI::Entity::UMLDomain iUMLAPI::Entity::currentDomain = None;

void iUMLAPI::Entity::setUMLDomain ( UMLDomain newDomain )
{
  if ( newDomain != currentDomain )
  {
    switch ( newDomain )
    {
      case ATT:  iUMLAPI::set_domain("ATT");  break; 
      case BS:   iUMLAPI::set_domain("BS");   break; 
      case ORG:  iUMLAPI::set_domain("ORG");  break; 
      case TAGS: iUMLAPI::set_domain("TAGS"); break; 
      case XUML: iUMLAPI::set_domain("XUML"); break; 
      case DM:   iUMLAPI::set_domain("DM");   break; 
      case DS:   iUMLAPI::set_domain("DS");   break; 
      case REQ:  iUMLAPI::set_domain("REQ");  break; 
      case SQ:   iUMLAPI::set_domain("SQ");   break; 
      case UC:   iUMLAPI::set_domain("UC");   break; 
      default:                                break;
    }
    currentDomain = newDomain;
  }
}


iUMLAPI::Entity::Attribute::Attribute ( const API_ATTRIBUTE& attribute ) : type(attribute.attr_type)
{
  switch ( type )
  {
    case API_INTEGER: longVal   = attribute.attr_value.v_int;  break;
    case API_BOOLEAN: boolVal   = attribute.attr_value.v_bool; break;
    case API_STRING:  stringVal = attribute.attr_value.v_str;  break;
  }
}

iUMLAPI::Entity::Attribute::Attribute ( const std::string& value ) : type(API_STRING), stringVal(value)
{
}

iUMLAPI::Entity::Attribute::Attribute ( bool value ) : type(API_BOOLEAN), boolVal(value)
{
}

iUMLAPI::Entity::Attribute::Attribute ( long value ) : type(API_INTEGER), longVal(value)
{
}

bool iUMLAPI::Entity::Attribute::getBool() const { if ( type != API_BOOLEAN ){std::cerr << "Cast error: Expected " << API_BOOLEAN  << " got " << type << std::endl; throw std::bad_cast(); } return boolVal; }
long iUMLAPI::Entity::Attribute::getLong() const { if ( type != API_INTEGER ) {std::cerr << "Cast error: Expected " << API_INTEGER  << " got " << type << std::endl; throw std::bad_cast(); } return longVal; }
const std::string& iUMLAPI::Entity::Attribute::getString() const { if ( type != API_STRING ) {std::cerr << "Cast error: Expected " << API_STRING  << " got " << type << std::endl; throw std::bad_cast(); } return stringVal; }

iUMLAPI::Entity::Attribute::operator API_ATTRIBUTE() const
{
  API_ATTRIBUTE result;
  result.attr_type = type;
  switch ( type )
  {
    case API_INTEGER: result.attr_value.v_int = longVal;  break;
    case API_BOOLEAN: result.attr_value.v_bool = boolVal; break;
    case API_STRING:  result.attr_value.v_str = const_cast<char*>(stringVal.c_str()); break;
  }
  return result;
}




