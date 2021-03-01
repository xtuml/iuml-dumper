// 
// Filename : Utils.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef iUMLAPI_Utils_HH
#define iUMLAPI_Utils_HH

#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/BS_BuildSet.hh"

namespace iUMLAPI
{

  const ORG::DomainVersion& getDomainVersion ( const XUML::AnalysisArea& analysisArea );
  const std::string& getDomainKeyLetter ( const XUML::AnalysisArea& analysisArea );
  const std::string& getDomainName ( const XUML::AnalysisArea& analysisArea );
  long getDomainNumber ( const XUML::AnalysisArea& analysisArea );
  long getDomainVersionNumber ( const XUML::AnalysisArea& analysisArea );


  const XUML::AnalysisArea& getAnalysisArea ( const ORG::DomainVersion& domainVersion );
 
}

#endif
