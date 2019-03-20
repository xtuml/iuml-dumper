// 
// Filename : ProjectTerminator.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_ProjectTerminator_HH
#define MaslGen_ProjectTerminator_HH

#include "iumlapi/XUML_NonCounterpartTerminator.hh"
#include "iumlapi/BS_NonCounterpartTerminator.hh"

#include "ProjectBridgeService.hh"
#include "ProjectBridgeEvent.hh"

#include "CodeFile.hh"

namespace MaslGen
{
  class ProjectTerminator
  {
    public:
      ProjectTerminator ( const iUMLAPI::BS::NonCounterpartTerminator& nct, const iUMLAPI::XUML::NonCounterpartTerminator& domainNct );

      void dumpToFiles() const;
      void updateFromFiles();
      std::string getPrjFileDeclaration(const std::string& indent) const;

    private:
      iUMLAPI::BS::NonCounterpartTerminator nct;
      iUMLAPI::XUML::NonCounterpartTerminator domainNct;

      mutable std::vector<ProjectBridgeEvent>     events;
      mutable std::vector<ProjectBridgeService>   services;
  };
}

#endif
