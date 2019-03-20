// 
// Filename : Identifier.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Identifier.hh"
#include "DataType.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_Attribute.hh"

#include "boost/lexical_cast.hpp"

MaslGen::Identifier::Identifier ( const iUMLAPI::XUML::Identifier& identifier )
  : identifier(identifier)
{
}

std::string MaslGen::Identifier::getModFileDeclaration( const std::string& indent ) const
{
  std::string result;

  if ( !identifier.getPreferred() )
  {
    result+= "\n" + indent + "identifier is (";

    std::string separator = "";

    for ( iUMLAPI::XUML::Attribute_Set::const_iterator it = identifier.getContainsAttributeR16().begin();
          it != identifier.getContainsAttributeR16().end();
          ++it )
    {
      result+= separator + makeIdentifier(it->getAttributeName());
      separator = ", ";
    }
    result+= ");\n";
  }
  return result;
}
