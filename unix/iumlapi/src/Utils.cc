// 
// Filename : Utils.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/Utils.hh"
#include "iumlapi/ORG_StandardDomainVersion.hh"
#include "iumlapi/ORG_PackageContainerDomainVersion.hh"
#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/ORG_Domain.hh"

const iUMLAPI::ORG::DomainVersion& iUMLAPI::getDomainVersion ( const iUMLAPI::XUML::AnalysisArea& analysisArea )
{
  if ( analysisArea.getORG_StandardDomainVersion().isValid() )
  {
    return analysisArea.
                  getORG_StandardDomainVersion().
                  getSupertypeDomainVersionR84();
  }
  else
  {
    return analysisArea.
                  getORG_PackageContainerDomainVersion().
                  getSupertypeDomainVersionR84();
  }

}

const iUMLAPI::XUML::AnalysisArea& iUMLAPI::getAnalysisArea ( const iUMLAPI::ORG::DomainVersion& domainVersion )
{
  if ( domainVersion.getSubtypeStandardDomainVersionR84().isValid() )
  {
    return domainVersion.
              getSubtypeStandardDomainVersionR84().
              getXUML_AnalysisArea();
  }
  else
  {
    return domainVersion.
              getSubtypePackageContainerDomainVersionR84().
              getXUML_AnalysisArea();
  }

}

const std::string& iUMLAPI::getDomainKeyLetter ( const iUMLAPI::XUML::AnalysisArea& analysisArea )
{
  return getDomainVersion(analysisArea).getIsAViewOfDomainR4().getKeyLetter();
}

long iUMLAPI::getDomainNumber ( const iUMLAPI::XUML::AnalysisArea& analysisArea )
{
  return getDomainVersion(analysisArea).getIsAViewOfDomainR4().getNumber();
}

