// 
// Filename : SuperSubtype.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "SuperSubtype.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_Generalisation.hh"
#include "iumlapi/XUML_Class.hh"

#include "boost/lexical_cast.hpp"

MaslGen::SuperSubtype::SuperSubtype ( const iUMLAPI::XUML::Generalisation& generalisation )
  : generalisation(generalisation)
{
}

std::string MaslGen::SuperSubtype::getModFileDeclaration( const std::string& indent ) const
{
  std::string result =
        indent + "relationship R" + boost::lexical_cast<std::string>(generalisation.getRelationshipNumber()) + " is " +
                                    makeIdentifier(generalisation.getHasSuperclassClassR12().getClassName()) + " is_a (";

  std::string separator = "\t";
  // Subclasses listed in reverse order to counter reversal in the MASL to xtUML importer DNS 6/8/20. 
  for ( iUMLAPI::XUML::Class_Set::const_reverse_iterator it = generalisation.getHasSubclassClassR13().rbegin();
        it != generalisation.getHasSubclassClassR13().rend();
        ++it )
  {
    result+= separator + makeIdentifier(it->getClassName());
    separator = ",\n\t";
  }

  result+= ");\n";

  return alignTabs(result);
}
