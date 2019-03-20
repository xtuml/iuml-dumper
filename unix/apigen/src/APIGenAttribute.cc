// 
// Filename : APIGenAttribute.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenAttribute.hh"
#include "APIGenUtils.hh"

#include <sstream>

APIGen::Attribute::Attribute ( const std::string& line )
{
  std::istringstream sline(line);
 
  char idChar;
  sline >> idChar;
  id = idChar == '*';

  std::getline(sline,name,',');
  std::getline(sline,type,',');
  std::getline(sline,reference);

  memberName = camelCase(name,false);
  getMethodName = "get" + camelCase(name,true);
  setMethodName = "set" + camelCase(name,true);

   if ( type == "Text" )
   {
     cppType = "std::string";
     cppRefType = "const std::string&";
     cppTypeGetMethod = "getString";
   }
   else if ( type == "Boolean" )
   {
     cppType = "bool";
     cppRefType = "bool";
     cppTypeGetMethod = "getBool";
   }
   else
   {
     cppType = "long";
     cppRefType = "long";
     cppTypeGetMethod = "getLong";
   }

}

std::ostream& APIGen::operator<< ( std::ostream& os, const APIGen::Attribute& attribute )
{
  os << std::boolalpha
     << "    <attribute "
     << "id=\"" << attribute.isId() << "\" " 
     << "name=\"" << attribute.getName() << "\" " 
     << "type=\"" << attribute.getType() << "\" "
     << "reference=\"" << attribute.getReference() << "\" "
     << "/>\n";
  return os;
}

