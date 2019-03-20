// 
// Filename : ProjectBridgeService.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_ProjectBridgeService_HH
#define MaslGen_ProjectBridgeService_HH

#include <string>
#include "iumlapi/XUML_NonCounterpartTerminatorOperation.hh"
#include "iumlapi/BS_TerminatorOperationBridge.hh"
#include "BridgeService.hh"

namespace MaslGen
{
  class ProjectBridgeService : public BridgeService
  {
    public:
      ProjectBridgeService ( const iUMLAPI::XUML::NonCounterpartTerminatorOperation& nctOp,
                             const iUMLAPI::BS::TerminatorOperationBridge& procOp );

      std::string getMethod() const;
      void updateMethod(const std::string& method);
      std::string getFileNamePrefix() const;
      bool hasCodeFile() const { return getMethod().size(); }
      virtual const TagSet& getTagSet() const { return projectTags; }
      virtual std::string getDescription() const;
      bool definedInProject() const;

    protected:
      TagSet projectTags;

    private:
      iUMLAPI::BS::TerminatorOperationBridge procOp;
  };
}

#endif
