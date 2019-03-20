// 
// Filename : APIGenDomain.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenDomain.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::map<std::string,APIGen::Domain> APIGen::Domains::domains;

APIGen::Domain& APIGen::Domains::getDomain( const std::string& name )
{
  std::map<std::string,Domain>::iterator it = domains.find(name);
  if ( it == domains.end() )
  {
    throw std::range_error("Domain not found: " + name);
  }
  return it->second;
}



APIGen::Domain::Domain ( const std::string& name_ ) : name(name_)
{
  std::string line;

  // Read Objects
  std::ifstream objectFile((name + ".obj").c_str());
  while ( std::getline(objectFile,line) )
  {
    if ( line != "" )
    {
      Object object(name,line);
      while ( std::getline(objectFile,line) && line != "" )
      {
        object.addAttribute(Attribute(line));
      }
      addObject(object);
    }
  }

  // Read Relationships
  std::ifstream relationshipFile((name + ".rel").c_str());
  while ( std::getline(relationshipFile,line) )
  {
    Relationship rel(line);
    getObject(rel.getFromObject()).addRelationship(rel);
  }

  // Read SuperSubtypes
  std::ifstream superSubtypesFile((name + ".sub").c_str());
  while ( std::getline(superSubtypesFile,line) )
  {
    std::istringstream sline(line);
    std::string relName;
    std::string supertype;
    std::string subtype;

    std::getline(sline,relName,',');
    std::getline(sline,supertype,',');

    while ( std::getline(sline,subtype,',') )
    {
      Relationship superRel(relName,supertype,subtype,true);
      Relationship subRel(relName,supertype,subtype,false);

      getObject(supertype).addRelationship(subRel);
      getObject(subtype).addRelationship(superRel);
    }
  }
  
  // Remove objects not supported by the API. ie those that 
  // are only associatives with no other useful information. 
  std::set<std::string> assocOnlyObjs;

  for ( std::map<std::string,APIGen::Object>::iterator obj = objects.begin(); 
        obj != objects.end(); )
  {
    if ( obj->second.isAssociativeOnly() )
    {
      assocOnlyObjs.insert(obj->second.getKeyLetters());
      objects.erase(obj++);
    }
    else
    {
      ++obj;
    }
  }

  // Remove relationships to objects just removed. 
  for ( std::map<std::string,APIGen::Object>::iterator obj = objects.begin(); 
        obj != objects.end(); ++obj )
  {
    obj->second.stripAssocOnlyRels(assocOnlyObjs);
  }

}

const APIGen::Object& APIGen::Domain::getObject( const std::string& kl ) const
{
  std::map<std::string,Object>::const_iterator it = objects.find(kl);
  if ( it == objects.end() )
  {
    throw std::range_error("Object not found: " + kl);
  }
  return it->second;
}

APIGen::Object& APIGen::Domain::getObject( const std::string& kl )
{
  std::map<std::string,Object>::iterator it = objects.find(kl);
  if ( it == objects.end() )
  {
    throw std::range_error("Object not found: " + kl);
  }
  return it->second;
}


void APIGen::Domain::generateAPI( const std::string & outputdir )
{
  for ( std::map<std::string,APIGen::Object>::iterator it = objects.begin(); 
        it != objects.end(); ++it )
  {
    try
    {
      it->second.generateAPI(outputdir);
    }
    catch ( const std::range_error& e )
    {
      std::cout << "Error generating : it->second.getFullName()" << e.what() << std::endl;
    }
  }
}


