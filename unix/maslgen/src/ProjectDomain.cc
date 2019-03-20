// 
// Filename : Project.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "ProjectDomain.hh"
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

MaslGen::ProjectDomain::ProjectDomain ( const iUMLAPI::BS::DomainVersionInBuildSet& domainVersion_, const iUMLAPI::ORG::Database& database )
  : domainVersion(domainVersion_)
{
  
  const iUMLAPI::ORG::Domain umlDomain = database.getDomain(domainVersion.getDomainKeyLetter());
  if ( umlDomain.getLatestVersionNo() != domainVersion.getDomainVersionNumber() )
  {
    std::cerr << "Warning: Using version " << domainVersion.getDomainVersionNumber() 
              << " of " << domainVersion.getDomainKeyLetter() << " when later version "
              << umlDomain.getLatestVersionNo() << " exists." << std::endl;
  }

  const iUMLAPI::XUML::AnalysisArea& analysisArea = 
          iUMLAPI::getAnalysisArea(umlDomain.getDomainVersion(domainVersion.getDomainVersionNumber()));

  // Create a lookup table to ease finding NCTs from the 
  // XUML model. A counterpart would be nice... 
  std::map<std::string,iUMLAPI::XUML::NonCounterpartTerminator> nctLookup;
  for ( iUMLAPI::XUML::NonCounterpartTerminator_Set::const_iterator it = analysisArea.getContainsNonCounterpartTerminatorR223().begin();
        it != analysisArea.getContainsNonCounterpartTerminatorR223().end();
        ++it )
  {
    nctLookup.insert(std::make_pair(it->getSupertypeSignalProducerConsumerR157().getKeyLetter(),*it));
  }

  const iUMLAPI::BS::NonCounterpartTerminator_Set& ncts = 
          domainVersion.getContainsNonCounterpartTerminatorR113();

  for ( iUMLAPI::BS::NonCounterpartTerminator_Set::const_iterator nct = 
              ncts.begin();
        nct != ncts.end();
        ++nct )
  {
    std::map<std::string,iUMLAPI::XUML::NonCounterpartTerminator>::const_iterator domainNCTIt = nctLookup.find(nct->getNonCounterpartTerminatorKeyLetter());

    if ( domainNCTIt == nctLookup.end() )
    {
      throw std::out_of_range("Non-Counterpart Terminator \"" + nct->getNonCounterpartTerminatorKeyLetter() + "\" not found in " + domainVersion.getDomainKeyLetter() + " version " + boost::lexical_cast<std::string>(domainVersion.getDomainVersionNumber()) + ".");
    }
    const iUMLAPI::XUML::NonCounterpartTerminator& domainNCT = domainNCTIt->second;

    terminators.push_back(ProjectTerminator(*nct, domainNCT));
  }

}

void MaslGen::ProjectDomain::updateFromFiles()
{
  std::for_each ( terminators.begin(), terminators.end(), std::mem_fun_ref(&ProjectTerminator::updateFromFiles) );
}

void MaslGen::ProjectDomain::dumpToFiles() const
{
  std::for_each ( terminators.begin(), terminators.end(), std::mem_fun_ref(&ProjectTerminator::dumpToFiles) );
}


std::string MaslGen::ProjectDomain::getPrjFileDeclaration( const std::string& indent ) const
{
  std::string prjFile;

  prjFile+= indent + "domain " + domainVersion.getDomainKeyLetter() + " is\n";

  for ( std::vector<ProjectTerminator>::const_iterator it = terminators.begin(); it != terminators.end(); ++it )
  {
    prjFile+= it->getPrjFileDeclaration(indent + "  ") + "\n";
  }

  prjFile+= indent + "end domain;\n";
  TagSet tags(domainVersion.getTAGS_TaggedItem());
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
        it != tags.getTags().end();
        ++it )
  {
    prjFile+= indent + "pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");\n";
  }
  prjFile+= "\n";

  return prjFile;

}
