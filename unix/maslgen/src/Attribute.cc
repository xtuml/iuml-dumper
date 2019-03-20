// 
// Filename : Attribute.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Attribute.hh"
#include "DataType.hh"
#include "TextUtils.hh"
#include "DumpOptions.hh"

#include "iumlapi/XUML_Identifier.hh"
#include "iumlapi/XUML_ReferentialAttribute.hh"
#include "iumlapi/XUML_ReferentialIdentifier.hh"
#include "iumlapi/XUML_AttributeReference.hh"
#include "iumlapi/XUML_FormalisedAssociation.hh"
#include "iumlapi/XUML_UMLAssociation.hh"
#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_SubclassParticipation.hh"
#include "iumlapi/XUML_Generalisation.hh"

#include "boost/lexical_cast.hpp"
// These should just be static members, but there appears to 
// be a bug in the sun linker/compiler, which crashes the 
// process when the member is accessed via a shared library. 
const std::string& MaslGen::Attribute::uniqueTag() { static std::string t ="unique"; return t; }

MaslGen::Attribute::Attribute ( const iUMLAPI::XUML::Attribute& attribute )
  : attribute(attribute),
    tags(attribute.getTAGS_TaggedItem())
{
}

enum RefType { RefA, RefB, Super };

std::string MaslGen::Attribute::getModFileDeclaration( const std::string& indent ) const
{
  std::string result =  wrapDescription(indent,attribute.getDescription(),60) + indent + makeIdentifier(attribute.getAttributeName()) + "\t : ";

  const iUMLAPI::XUML::Identifier_Set& identifiers = attribute.getIsPartOfIdentifierR16();
  
  bool preferred = false;

  for ( iUMLAPI::XUML::Identifier_Set::const_iterator it = identifiers.begin();
        it != identifiers.end() && !preferred;
        ++it )
  {
    preferred |= it->getPreferred();
  }

  if ( preferred )
  {
    result += "preferred ";
  }

  if ( tags.hasTag(uniqueTag()) )
  {
    result += "unique ";
  }

  const iUMLAPI::XUML::AttributeReference_Set& attRefs = attribute.getSubtypeReferentialAttributeR10().getRefersToAttributeReferenceR23();

  if ( attRefs.size() )
  {
       if (!DumpOptions::instance().hasNoRelShips()){
           result += "referential (";
           std::string separator = "";

           for ( iUMLAPI::XUML::AttributeReference_Set::const_iterator it = attRefs.begin();
                 it != attRefs.end();
                 ++it )
           {
               result+= separator;
               const iUMLAPI::XUML::ReferentialIdentifier& refId = it->getBelongsToReferentialIdentifierR28();
               RefType type = static_cast<RefType>(refId.getRelationshipEnd());
               switch ( type )
               {
                 case RefA:
                 {
                   const iUMLAPI::XUML::UMLAssociation assoc = 
                            refId.getReferencesAnIdentifierAtTheAEndOfFormalisedAssociationR21().
                                getSupertypeUMLAssociationR7();
                   std::string role = makeIdentifier(assoc.getAEndRoleName());
                   if ( role == "" ) role = "WARNING_undefined_role_name";
                   result+= "R" + boost::lexical_cast<std::string>(assoc.getRelationshipNumber()) +
                           "." + role + 
                           "." + makeIdentifier(assoc.getHasAnAEndOfClassR2().getClassName()) + ".";
                 } break;
                 case RefB:
                 {
                   const iUMLAPI::XUML::UMLAssociation assoc = 
                            refId.getReferencesAnIdentifierAtTheBEndOfFormalisedAssociationR22().
                                getSupertypeUMLAssociationR7();
                   std::string role = makeIdentifier(assoc.getBEndRoleName());
                   if ( role == "" ) role = "WARNING_undefined_role_name";
                   result+= "R" + boost::lexical_cast<std::string>(assoc.getRelationshipNumber()) +
                           "." + role + 
                           "." + makeIdentifier(assoc.getHasABEndOfClassR3().getClassName()) + ".";
                 } break;
                 case Super:
                 {
                   result+= "R" + boost::lexical_cast<std::string>(refId.getFormalisesSubclassParticipationR220().
                                getIsSubclassInGeneralisationR13().getRelationshipNumber()) + ".";
                 } break;
                 default:
                   result+= "ERROR";
               }
               result+= makeIdentifier(it->getRefersToAttributeR23().getAttributeName());
               separator = ", ";
            }
            result+= ") ";
         }
  }
  result+= DataType(attribute.getHasItsDomainSpecifiedByDataTypeR71()).getName();

  if ( attribute.getDefaultValue() != "" )
  {
    result += " := " + attribute.getDefaultValue();
  }

  result+=  ";" + getPragmas() + "\n";

  return result;
}

std::string MaslGen::Attribute::getPragmas() const
{
  std::string result;

  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
        it != tags.getTags().end();
        ++it )
  {
    if ( it->first != uniqueTag() )
    {
      result+= " pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");";
    }
  }

  return result;
}

