// 
// Filename : ProjectBridgeEvent.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "ProjectBridgeEvent.hh"
#include "iumlapi/BS_NonCounterpartTerminator.hh"
#include "iumlapi/BS_DomainVersionInBuildSet.hh"
#include "TextUtils.hh"

MaslGen::ProjectBridgeEvent::ProjectBridgeEvent ( 
            const iUMLAPI::XUML::TerminatorSignal& termSig,
            const iUMLAPI::BS::TerminatorSignalBridge& procSig )
  : BridgeEvent(termSig),
    procSig(procSig),
    projectTags(procSig.getTAGS_TaggedItem())
{
}

std::string MaslGen::ProjectBridgeEvent::getMethod() const
{
  return procSig.getBridgeMethod();
}

void MaslGen::ProjectBridgeEvent::updateMethod( const std::string& method )
{
  return procSig.setBridgeMethod(method);
}

std::string MaslGen::ProjectBridgeEvent::getFileNamePrefix() const
{
  return procSig.getDefinesSignalOperationBridgeRequirementForNonCounterpartTerminatorR32().
                getIsDefinedWithinDomainVersionInBuildSetR113().
                getDomainKeyLetter() + "_" + procSig.getDefinesSignalOperationBridgeRequirementForNonCounterpartTerminatorR32().getNonCounterpartTerminatorKeyLetter() + "_";
}

std::string MaslGen::ProjectBridgeEvent::getDescription() const
{
  return procSig.getContractDescription();
}

bool MaslGen::ProjectBridgeEvent::definedInProject() const
{
  return isNative() || getMethod().size() > 0 || getTagSet().getTags().size() > 0;
}
