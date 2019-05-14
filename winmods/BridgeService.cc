// 
// Filename : BridgeService.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "BridgeService.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_SignalProducerConsumer.hh"

MaslGen::BridgeService::BridgeService ( const iUMLAPI::XUML::NonCounterpartTerminatorOperation& nctOp )
  : Service(nctOp.getSupertypeOperationR215()),
    nct(nctOp.getIsProvidedByNonCounterpartTerminatorR218()),
	nctOp(nctOp)
{
}

std::string MaslGen::BridgeService::getFileNamePrefix() const
{
  return makeIdentifier(nct.getSupertypeSignalProducerConsumerR157().getKeyLetter()==""?nct.getName():nct.getSupertypeSignalProducerConsumerR157().getKeyLetter()) + "_";
}

std::string MaslGen::BridgeService::getFileNameExtension() const
{
  return "tr";
}

std::string MaslGen::BridgeService::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(nct.getName()) + "~>";
}

std::string MaslGen::BridgeService::getUnqualifiedName() const
{
  return  Service::getUnqualifiedName();
}

long MaslGen::BridgeService::getOperationNumber() const
{
  return nctOp.getSupertypeOperationR215().getOperationNumber();
}
