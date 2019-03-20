// 
// Filename : APIGenRelationship.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenRelationship.hh"

#include <sstream>

APIGen::Relationship::Relationship ( const std::string& line )
  : superSub(false)
{
  std::istringstream sline(line);

  std::string conditionality;
  std::string cardinality;
  std::string isassoc;
 
  std::getline(sline,name,',');
  std::getline(sline,fromObject,',');
  std::getline(sline,conditionality,',');
  std::getline(sline,role,',');
  std::getline(sline,cardinality,',');
  std::getline(sline,toObject,',');
  std::getline(sline,isassoc);

  many = cardinality == "many";
  conditional = conditionality == "conditionally";
  assoc = isassoc == "isassoc";

}

APIGen::Relationship::Relationship ( const std::string& name, const std::string& supertype, const std::string& subtype, bool toSuper )
  : name(name),
    fromObject(toSuper?subtype:supertype),
    conditional(toSuper?false:true),
    role(toSuper?"supertype":"subtype"),
    many(false),
    toObject(toSuper?supertype:subtype),
    assoc(false),
    superSub(true)
{
    
}

std::ostream& APIGen::operator<< ( std::ostream& os, const APIGen::Relationship& rel )
{
  os << std::boolalpha
     << "    <relationship "
     << "name=\"" << rel.getName() << "\" " 
     << "conditional=\"" << rel.isConditional() << "\" "
     << "role=\"" << rel.getRole() << "\" "
     << "many=\"" << rel.isMany() << "\" "
     << "to=\"" << rel.getToObject() << "\" "
     << "assoc=\"" << rel.isAssoc() << "\" "
     << "/>\n";
  return os;
}

