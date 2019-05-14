// 
// Filename : Service.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Service.hh"
#include "TextUtils.hh"
#include "Parameter.hh"
#include "DataType.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_OperationParameter.hh"
#include "iumlapi/XUML_UserDefinedDataType.hh"
#include "iumlapi/XUML_Attribute.hh"

#include <iterator>

// These should just be static members, but there appears to 
// be a bug in the sun linker/compiler, which crashes the 
// process when the member is accessed via a shared library. 
const std::string& MaslGen::Service::nativeTag()  { static std::string t ="native"; return t; }
const std::string& MaslGen::Service::functionTag() { static std::string t ="function"; return t; }
const std::string& MaslGen::Service::visibilityTag() { static std::string t ="visibility"; return t; }

MaslGen::Service::Service ( const iUMLAPI::XUML::Operation& operation )
  : operation(operation),
    tags(operation.getTAGS_TaggedItem())
{
}

bool MaslGen::Service::isNative() const
{
  return getTagSet().hasTag(nativeTag());
}

bool MaslGen::Service::isPublic() const
{
  if ( getTagSet().hasTag(visibilityTag()) )
  {
    return !(getTagSet().getTag(visibilityTag()) == "private" );
  }
  else
  {
    return true;
  }
}

bool MaslGen::Service::isFunction() const
{
  return getTagSet().hasTag(functionTag());
}

std::string MaslGen::Service::getPragmas() const
{
  std::string result;

  if ( operation.getName().find_first_of('.') != std::string::npos )
  {
    result += " pragma filename (\"" + getFileName() + "\");";
  }

  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = getTagSet().getTags().begin();
        it != getTagSet().getTags().end();
        ++it )
  {
    if ( it->first != nativeTag() && it->first != functionTag() && it->first != visibilityTag() )
    {
      result+= " pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");";
    }
  }

  return result;
}


std::string MaslGen::Service::getCallableType() const
{
  return std::string((isPublic()?"public":"private")) + " " + (isFunction()?"function":"service");
}

std::string MaslGen::Service::getUnqualifiedName() const
{
  std::string fullName = operation.getName();
  std::string::size_type dotPos = fullName.find_first_of('.');
  if ( dotPos == std::string::npos )
  {
    return makeIdentifier(fullName);
  }
  else
  {
    return makeIdentifier(fullName.substr(0,dotPos));
  }
}

std::string MaslGen::Service::getDomainName() const
{
  return iUMLAPI::getDomainKeyLetter(operation.getIsDefinedInAnalysisAreaR238());
}

std::string MaslGen::Service::getFileNameExtension() const
{
  std::string fullName = operation.getName();
  std::string::size_type dotPos = fullName.find_first_of('.');
  if ( dotPos == std::string::npos )
  {
    return isFunction()?"fn":"svc";
  }
  else
  {
    return makeIdentifier(fullName.substr(dotPos+1)) + "." + (isFunction()?"fn":"svc");
  }
}

std::string MaslGen::Service::getDescription() const
{
  return operation.getDescription();
}

std::string MaslGen::Service::getParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const
{
  std::vector<DataType> types;

  std::copy ( operation.getHasInputOperationParameterR201().getHasTypeDataTypeR207().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              operation.getHasInputOperationParameterR201().getHasTypeDataTypeR207().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::copy ( operation.getHasOutputOperationParameterR202().getHasTypeDataTypeR207().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              operation.getHasOutputOperationParameterR202().getHasTypeDataTypeR207().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::copy ( operation.getHasInputOperationParameterR201().getHasNameAndOrTypeLinkedToAttributeR203().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              operation.getHasInputOperationParameterR201().getHasNameAndOrTypeLinkedToAttributeR203().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::copy ( operation.getHasOutputOperationParameterR202().getHasNameAndOrTypeLinkedToAttributeR203().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              operation.getHasOutputOperationParameterR202().getHasNameAndOrTypeLinkedToAttributeR203().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::string definition;
  for ( std::vector<DataType>::const_iterator it = types.begin();
        it != types.end();
        ++it )
  {
     definition += it->getModFileFullDefinition(indent,alreadyDefined,isPublic());
  }
  return definition;
}



std::string MaslGen::Service::getParameterList() const
{
  std::vector<Parameter> inParameters;
  std::vector<Parameter> outParameters;

  std::copy ( operation.getHasInputOperationParameterR201().begin(), 
              operation.getHasInputOperationParameterR201().end(), 
              std::back_inserter(inParameters) );

  std::copy ( operation.getHasOutputOperationParameterR202().begin(), 
              operation.getHasOutputOperationParameterR202().end(), 
              std::back_inserter(outParameters) );
 
  std::string paramList = "(";
  std::string prefix = "\t";

  for ( std::vector<Parameter>::const_iterator it = inParameters.begin();
        it != inParameters.end(); ++it )
  {
    paramList += prefix + it->getName() + " \t: in  " + it->getType();  
    prefix = ",\n\t";
  }

  std::vector<Parameter>::iterator it = outParameters.begin();
  if ( isFunction() && outParameters.size() > 0 ) ++it;

  for ( /* init above */ ;it != outParameters.end(); ++it )
  {
    paramList += prefix + it->getName() + " \t: out " + it->getType();  
    prefix = ",\n\t";
  }

  paramList += ")";

  if ( isFunction() && outParameters.size() > 0 )
  {
    paramList+= std::string(" return ") + outParameters[0].getType();
  }

  return paramList;

}

std::string MaslGen::Service::getMethod() const
{
  return operation.getMethod();
}

void MaslGen::Service::updateMethod(const std::string& method)
{
  operation.setMethod(method);
}

