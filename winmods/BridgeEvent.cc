// 
// Filename : BridgeEvent.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "BridgeEvent.hh"
#include "TextUtils.hh"
#include "DataType.hh"

#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_NonCounterpartTerminator.hh"
#include "iumlapi/XUML_SignalParameter.hh"
#include "iumlapi/XUML_UserDefinedDataType.hh"
#include "iumlapi/XUML_Attribute.hh"

#include <iterator>

const std::string& MaslGen::BridgeEvent::nativeTag()  { static std::string t ="native"; return t; }

MaslGen::BridgeEvent::BridgeEvent ( const iUMLAPI::XUML::TerminatorSignal& termSig )
  : EventAction(termSig.getSupertypeUMLSignalR128()),
    termSig(termSig),
    tags(termSig.getTAGS_TaggedItem())
{
}

std::string MaslGen::BridgeEvent::getFileNamePrefix() const
{
  return makeIdentifier(termSig.getSupertypeUMLSignalR128().getIsDirectedAtSignalProducerConsumerR166().getKeyLetter() == ""?
                        termSig.getSupertypeUMLSignalR128().getIsDirectedAtSignalProducerConsumerR166().getSubtypeNonCounterpartTerminatorR157().getName():
                        termSig.getSupertypeUMLSignalR128().getIsDirectedAtSignalProducerConsumerR166().getKeyLetter()) + "_";
}

std::string MaslGen::BridgeEvent::getFileNameExtension() const
{
  return "tr";
}

std::string MaslGen::BridgeEvent::getCallableType() const
{
  return std::string("public service");
}

std::string MaslGen::BridgeEvent::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(termSig.getSupertypeUMLSignalR128().getIsDirectedAtSignalProducerConsumerR166().getSubtypeNonCounterpartTerminatorR157().getName()) + "~>";
}


std::string MaslGen::BridgeEvent::getUnqualifiedName() const
{
  return EventAction::getUnqualifiedName();
}

std::string MaslGen::BridgeEvent::getMethod() const
{
  return termSig.getMethod();
}

void MaslGen::BridgeEvent::updateMethod( const std::string& method )
{
  return termSig.setMethod(method);
}

std::string MaslGen::BridgeEvent::getPragmas() const
{
  std::string result;

  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = getTagSet().getTags().begin();
        it != getTagSet().getTags().end();
        ++it )
  {
    result+= " pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");";
  }

  return result;
}

bool MaslGen::BridgeEvent::isNative() const
{
  return getTagSet().hasTag(nativeTag());
}

std::string MaslGen::BridgeEvent::getParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const
{
  std::vector<DataType> types;

  std::copy ( termSig.getSupertypeUMLSignalR128().getCarriesSupplementarySignalParameterR151().getHasTypeDataTypeR191().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              termSig.getSupertypeUMLSignalR128().getCarriesSupplementarySignalParameterR151().getHasTypeDataTypeR191().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::copy ( termSig.getSupertypeUMLSignalR128().getCarriesSupplementarySignalParameterR151().getHasNameAndOrTypeLinkedToAttributeR192().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(), 
              termSig.getSupertypeUMLSignalR128().getCarriesSupplementarySignalParameterR151().getHasNameAndOrTypeLinkedToAttributeR192().getHasItsDomainSpecifiedByDataTypeR71().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(), 
              std::back_inserter(types) );

  std::string definition;
  for ( std::vector<DataType>::const_iterator it = types.begin();
        it != types.end();
        ++it )
  {
     definition += it->getModFileFullDefinition(indent,alreadyDefined,true);
  }
  return definition;
}

