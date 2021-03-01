// 
// Filename : DataType.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "DataType.hh"
#include "TextUtils.hh"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#include "iumlapi/XUML_UserDefinedDataType.hh"
#include "iumlapi/XUML_DataStructureType.hh"
#include "iumlapi/XUML_DataStructureMember.hh"
#include "iumlapi/XUML_EnumerationType.hh"
#include "iumlapi/XUML_EnumerationElement.hh"
#include "iumlapi/XUML_Attribute.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

enum Type { Instance = 1, Structure, TimeOfDay, Date, Real, Integer, Text, Deferred, Enumeration };
const std::string& MaslGen::DataType::visibilityTag() { static std::string t ="visibility"; return t; }

MaslGen::DataType::DataType ( const iUMLAPI::XUML::DataType& dataType_ )
  : dataType(dataType_),
    tags(dataType_.getSubtypeUserDefinedDataTypeR73().getTAGS_TaggedItem())
{
  std::string apiName = dataType.getDataTypeName();
  const iUMLAPI::XUML::UserDefinedDataType& udt = dataType.getSubtypeUserDefinedDataTypeR73();
  if ( !udt.isValid() )
  {
    if ( apiName == "Instance Handle" || apiName == "Untyped Instance Handle" ) name = "instance";
    else if ( apiName == "Event Handle" ) name = "event";
    else if ( apiName == "Integer" ) name = "integer";
    else if ( apiName == "Text" ) name = "string";
    else if ( apiName == "Real" ) name = "real";
    else if ( apiName == "Boolean" ) name = "boolean";
    else if ( apiName == "Date" ) name = "timestamp";
    else if ( apiName == "Time of Day" ) name = "duration";
    else name = apiName;
  }
  else
  {
    constraint = udt.getConstraint();
    if ( udt.getDataTypeSubclass() == Instance )
    {
      name = "instance of " + makeIdentifier(apiName);
    }
    else if ( udt.getDataTypeSubclass() == Deferred && constraint == "" )
    {
      name = apiName;
    }
    else
    {
      name = makeIdentifier(apiName);
    }
  }
}

bool MaslGen::DataType::isPublic() const
{
  if ( getTagSet().hasTag(visibilityTag()) )
  {
    return !(getTagSet().getTag(visibilityTag()) == "private" );
  }
  else
  {
    return false;
  }
}

std::string MaslGen::DataType::getModFileFullDefinition ( const std::string& indent, std::set<std::string>& alreadyDefined, bool forcePublic ) const
{

  std::string definition;
  if ( !alreadyDefined.count(getInternalName()) )
  {
    for ( std::vector<DataType>::const_iterator it = getContainedTypes().begin();
          it != getContainedTypes().end();
          ++it )
    {
      definition += it->getModFileFullDefinition(indent, alreadyDefined, forcePublic || isPublic() );
    }

    if ( dataType.getSubtypeUserDefinedDataTypeR73().getDataTypeSubclass() == Deferred )
    {
      std::string ofStr = " of ";  
      std::string::size_type ofStart = constraint.rfind(ofStr);

      std::string::size_type typeEnd = constraint.find(";");
      if ( ofStart != std::string::npos && typeEnd != std::string::npos )
      {
        std::string sequenceName(constraint.substr(ofStart+ofStr.length(),typeEnd-ofStart-ofStr.length()));

        const iUMLAPI::XUML::DataType_Set& allUDTs = 
                dataType.getIsDefinedInAnalysisAreaR221().
                         getContainsDataTypeR221().
                         getSubtypeUserDefinedDataTypeR73().
                         getSupertypeDataTypeR73();
        for ( iUMLAPI::XUML::DataType_Set::const_iterator it = allUDTs.begin();
              it != allUDTs.end(); ++it )
        {
          DataType candidate(*it);
          if ( makeIdentifier(candidate.getName()) == sequenceName )
          {
            definition += candidate.getModFileFullDefinition(indent, alreadyDefined, forcePublic || isPublic() );
            break;
          }
        }
      }
    }


    definition += getModFileDefinition(indent, forcePublic);
    alreadyDefined.insert(getInternalName());
  }
  return definition;
}


std::string MaslGen::DataType::getModFileDefinition(const std::string& indent, bool forcePublic ) const
{
  const iUMLAPI::XUML::UserDefinedDataType& udt = dataType.getSubtypeUserDefinedDataTypeR73();

  std::string def = wrapDescription(indent,udt.getDescription(),1000) + 
                    indent + ((forcePublic||isPublic())?"public":"private") + " type " + makeIdentifier(dataType.getDataTypeName()) + " is ";
  std::string              constraint = "";
  std::vector<std::string> split_constraint;

  switch ( udt.getDataTypeSubclass() )
  {
    case Instance:
      return "";

    case Structure:
    {
      def += "structure\n";
      const iUMLAPI::XUML::DataStructureMember_Set& elements =
                      udt.getSubtypeDataStructureTypeR74().
                          getContainsDataStructureMemberR189();
      for ( iUMLAPI::XUML::DataStructureMember_Set::const_iterator it = elements.begin();
            it != elements.end();
            ++it )
      { 
        const iUMLAPI::XUML::DataType& eltType = it->getHasTypeDataTypeR185().isValid()
                                               ? it->getHasTypeDataTypeR185()
                                               : it->getHasNameAndOrTypeLinkedToAttributeR184().
                                                     getHasItsDomainSpecifiedByDataTypeR71();

        def+= indent + "  " + makeIdentifier(it->getStructureMemberName()) + " \t: " + 
              (it->getIsASet()?"sequence of ":"") + DataType(eltType).getName() + ";\n";
      }
      def+= indent + "end structure;";
      def = alignTabs(def);
    } break;

    case TimeOfDay:
      def+= "duration;";
      break;
    
    case Date:
      def+= "timestamp;";
      break;
    
    case Real:
      def+= "real;";
      constraint = udt.getConstraint();
	  if ( constraint != "" )
	  {
         trim_if(constraint, boost::is_any_of("\n\r\t ")); // split can't omit separators or line feeds at ends
		 split(split_constraint, constraint, boost::is_any_of(" "), boost::token_compress_on);
	     def+= " pragma type_range(" + split_constraint.front() + "," + split_constraint.back() + ");";
	  }
      break;

    case Integer:
      def+= "integer;";
	  constraint = udt.getConstraint();
	  if ( constraint != "" )
	  {
         trim_if(constraint, boost::is_any_of("\n\r\t ")); // split can't omit separators or line feeds at ends
         split(split_constraint, constraint, boost::is_any_of(" "), boost::token_compress_on);
	     def+= " pragma type_range(" + split_constraint.front() + "," + split_constraint.back() + ");";
	  }
      break;

    case Text:
      def+= "string;";
      break;

    case Deferred:
    {
      if ( constraint == "" )
      {
        return "";
      }

      def = indentText(indent,((forcePublic||isPublic())?"public ":"private ") + constraint);
    } break;
 
    case Enumeration:
    {
      if ( ( dataType.getDataTypeName() == "EXCEPTION OBJECT" ) ||
           ( dataType.getDataTypeName() == "EXCEPTION TYPE" ) ||
           ( dataType.getDataTypeName() == "EXCEPTION STATE" ) )
      {
        return "";
      }

      def += "enum (";
      std::string separator = "\t";
      const iUMLAPI::XUML::EnumerationElement_Set& enums = udt.getSubtypeEnumerationTypeR74().getIsDerivedOfEnumerationElementR75();
      for ( iUMLAPI::XUML::EnumerationElement_Set::const_iterator it = enums.begin();
            it != enums.end();
            ++it )
      {
        def += separator + makeIdentifier(it->getEnumerationName());    
      
        if ( it->getEnumerationValueSet() )
        {
          def+= " \t= " + boost::lexical_cast<std::string>(it->getEnumerationValue());
        }
        separator = ",\n\t";
      }
      def += ");";
      def = alignTabs(def);
    } break;

  }

  return def + "\n\n";
}

const std::vector<MaslGen::DataType>& MaslGen::DataType::getContainedTypes() const
{
  if ( !ctInitialised )
  {
    ctInitialised = true;

    const iUMLAPI::XUML::DataType_Set& contained1 = 
           dataType.getSubtypeUserDefinedDataTypeR73().
                    getSubtypeDataStructureTypeR74().
                    getContainsDataStructureMemberR189().
                    getHasTypeDataTypeR185().
                    getSubtypeUserDefinedDataTypeR73().
                    getSupertypeDataTypeR73();
    std::copy(contained1.begin(),contained1.end(),std::back_inserter(containedTypes));

    const iUMLAPI::XUML::DataType_Set& contained2 = 
           dataType.getSubtypeUserDefinedDataTypeR73().
                    getSubtypeDataStructureTypeR74().
                    getContainsDataStructureMemberR189().
                    getHasNameAndOrTypeLinkedToAttributeR184().
                    getHasItsDomainSpecifiedByDataTypeR71().
                    getSubtypeUserDefinedDataTypeR73().
                    getSupertypeDataTypeR73();
    std::copy(contained2.begin(),contained2.end(),std::back_inserter(containedTypes));

  }
  return containedTypes;
}

