// 
// Filename : Project.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "ProjectTerminator.hh"
#include "TextUtils.hh"
#include "TagSet.hh"

#include "iumlapi/BS_DomainVersionInBuildSet.hh"
#include "iumlapi/BS_TerminatorSignalBridge.hh"
#include "iumlapi/BS_TerminatorOperationBridge.hh"
#include "iumlapi/BS_NonCounterpartTerminator.hh"

#include "iumlapi/ORG_Database.hh"
#include "iumlapi/ORG_Domain.hh"
#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/ORG_StandardDomainVersion.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_Operation.hh"
#include "iumlapi/XUML_NonCounterpartTerminatorOperation.hh"
#include "iumlapi/XUML_NonCounterpartTerminator.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_TerminatorSignal.hh"

#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include <map>

MaslGen::ProjectTerminator::ProjectTerminator ( const iUMLAPI::BS::NonCounterpartTerminator& nct_, const iUMLAPI::XUML::NonCounterpartTerminator& domainNct_ )
  : nct(nct_),
    domainNct(domainNct_)
{
  
  // Create lookup tables to ease finding bridge events 
  // and services from the XUML model. Counterparts? Chance would be a fine thing. 
  std::map<long,iUMLAPI::XUML::TerminatorSignal> signalLookup;
  const iUMLAPI::XUML::TerminatorSignal_Set& domainSignals = domainNct.getSupertypeSignalProducerConsumerR157().
                                                                       getIsTargetOfUMLSignalR166().
                                                                       getSubtypeTerminatorSignalR128();
  for ( iUMLAPI::XUML::TerminatorSignal_Set::const_iterator it = domainSignals.begin();
        it != domainSignals.end();
        ++it )
  {
    signalLookup.insert(std::make_pair(it->getSupertypeUMLSignalR128().getSignalNumber(),*it));
  }

  std::map<long,iUMLAPI::XUML::NonCounterpartTerminatorOperation> operationLookup;
  const iUMLAPI::XUML::NonCounterpartTerminatorOperation_Set& domainOps = domainNct.getProvidesNonCounterpartTerminatorOperationR218();
  for ( iUMLAPI::XUML::NonCounterpartTerminatorOperation_Set::const_iterator it = domainOps.begin();
        it != domainOps.end();
        ++it )
  {
    operationLookup.insert(std::make_pair(boost::lexical_cast<long>(it->getSupertypeOperationR215().getOperationNumber()),*it));
  }

  // Dump the Bridge Events
  const iUMLAPI::BS::TerminatorSignalBridge_Set& bridgeSigs = 
          nct.getHasNonCounterpartSignalBridgeOperationRequirementsDefinedByTerminatorSignalBridgeR32();

  for ( iUMLAPI::BS::TerminatorSignalBridge_Set::const_iterator it = 
              bridgeSigs.begin();
        it != bridgeSigs.end();
        ++it )
  {
    std::map<long,iUMLAPI::XUML::TerminatorSignal>::const_iterator signal = signalLookup.find(it->getSignalNumber());

    if ( signal == signalLookup.end() )
    {
      throw std::out_of_range("Terminator Signal \"" + it->getSignalName() + "\" not found in " + nct.getNonCounterpartTerminatorKeyLetter() + ".");
    }

    events.push_back(ProjectBridgeEvent(signal->second,*it));
  }

  // Dump the Bridge Services
  const iUMLAPI::BS::TerminatorOperationBridge_Set& bridgeOps = 
           nct.getHasNonCounterpartSynchronousBridgeOperationRequirementsDefinedByTerminatorOperationBridgeR33();

  for ( iUMLAPI::BS::TerminatorOperationBridge_Set::const_iterator it = 
              bridgeOps.begin();
        it != bridgeOps.end();
        ++it )
  {
    std::map<long,iUMLAPI::XUML::NonCounterpartTerminatorOperation>::const_iterator operation = operationLookup.find(it->getTerminatorOperationNumber());

    if ( operation == operationLookup.end() )
    {
      throw std::out_of_range("Terminator Operation \"" + it->getTerminatorOperationName() + "\" not found in " + nct.getNonCounterpartTerminatorKeyLetter() + ".");
    }

    services.push_back(ProjectBridgeService(operation->second,*it));
  }

}

void MaslGen::ProjectTerminator::updateFromFiles()
{
  std::for_each ( services.begin(), services.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( events.begin(), events.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
}

void MaslGen::ProjectTerminator::dumpToFiles() const
{
  std::for_each ( services.begin(), services.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( events.begin(), events.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );

}


std::string MaslGen::ProjectTerminator::getPrjFileDeclaration( const std::string& indent ) const
{
  std::string serviceText;

  for ( std::vector<ProjectBridgeService>::const_iterator it = services.begin(); it != services.end(); ++it )
  {
    if ( it->definedInProject() )
    {
      serviceText+= it->getModFileDeclaration(indent + "  ") + "\n";
    }
  }

  for ( std::vector<ProjectBridgeEvent>::const_iterator it = events.begin(); it != events.end(); ++it )
  {
    if ( it->definedInProject() )
    {
      serviceText+= it->getModFileDeclaration(indent + "  ") + "\n";
    }
  }

  std::string prjFile;
  TagSet tags(nct.getTAGS_TaggedItem());
  if ( serviceText.size() > 0 || tags.getTags().size() > 0 )
  {
    prjFile+= indent + "terminator " + makeIdentifier(domainNct.getName()) + " is\n" +
              serviceText +
              indent + "end terminator;\n" + 
              indent + "pragma key_letter (\"" + nct.getNonCounterpartTerminatorKeyLetter() + "\");\n";
    for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
          it != tags.getTags().end();
          ++it )
    {
      prjFile+= indent + "pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");\n";
    }
    prjFile+= "\n";
  }
  return prjFile;

}
