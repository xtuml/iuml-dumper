// 
// Filename : Relationship.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Relationship.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_UMLAssociation.hh"
#include "iumlapi/XUML_AssociationClass.hh"
#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_FormalisedAssociation.hh"

#include "boost/lexical_cast.hpp"

MaslGen::Relationship::Relationship ( const iUMLAPI::XUML::UMLAssociation& association )
  : association(association)
{
}

std::string MaslGen::Relationship::getModFileDeclaration( const std::string& indent ) const
{
  std::string aClass = makeIdentifier(association.getHasAnAEndOfClassR2().getClassName());
  std::string bClass = makeIdentifier(association.getHasABEndOfClassR3().getClassName());

  std::string aRole  = makeIdentifier(association.getAEndRoleName());
  std::string bRole  = makeIdentifier(association.getBEndRoleName());

  if ( aRole == "" ) aRole = "WARNING_undefined_role_name";
  if ( bRole == "" ) bRole = "WARNING_undefined_role_name";

  std::string result =
        wrapDescription(indent,association.getDescription(),60) + 
        indent + "relationship R" + boost::lexical_cast<std::string>(association.getRelationshipNumber()) + " is \t" +

                                   aClass + " " + 
                                  (association.getBEndConditionality()?"conditionally":"unconditionally") + " " +
                                   bRole + " " +
                                  (association.getBEndMultiplicity()?"many":"one") + " " +
                                   bClass + ",\n\t" + 

                                   bClass + " " + 
                                  (association.getAEndConditionality()?"conditionally":"unconditionally") + " " +
                                   aRole + " " +
                                  (association.getAEndMultiplicity()?"many":"one") + " " +
                                   aClass;

  const iUMLAPI::XUML::AssociationClass& assocClass =
      association.getSubtypeFormalisedAssociationR7().
                  getIsFormalisedInAssociationClassR29();
  if ( assocClass.isValid() )
  {
    result+= std::string("\n\tusing ") + makeIdentifier(assocClass.getSupertypeClassR5().getClassName());
  }

  result+= ";\n";

  return alignTabs(result);
}
