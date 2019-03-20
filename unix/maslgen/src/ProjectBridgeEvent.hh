// 
// Filename : ProjectBridgeEvent.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_ProjectBridgeEvent_HH
#define MaslGen_ProjectBridgeEvent_HH

#include <string>
#include "iumlapi/XUML_TerminatorSignal.hh"
#include "iumlapi/BS_TerminatorSignalBridge.hh"
#include "BridgeEvent.hh"
#include "TagSet.hh"

namespace MaslGen
{
  class ProjectBridgeEvent : public BridgeEvent
  {
    public:
      ProjectBridgeEvent ( const iUMLAPI::XUML::TerminatorSignal& termSig,
                             const iUMLAPI::BS::TerminatorSignalBridge& procSig );

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
      iUMLAPI::BS::TerminatorSignalBridge procSig;
  };
}

#endif
