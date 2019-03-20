// 
// Filename : ProjectBridgeService.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "ProjectBridgeService.hh"
#include "iumlapi/BS_NonCounterpartTerminator.hh"
#include "iumlapi/BS_DomainVersionInBuildSet.hh"
#include "TextUtils.hh"

MaslGen::ProjectBridgeService::ProjectBridgeService ( 
            const iUMLAPI::XUML::NonCounterpartTerminatorOperation& nctOp,
            const iUMLAPI::BS::TerminatorOperationBridge& procOp )
  : BridgeService(nctOp),
    procOp(procOp),
    projectTags(procOp.getTAGS_TaggedItem())
{
}

std::string MaslGen::ProjectBridgeService::getMethod() const
{
  return procOp.getBridgeMethod();
}

void MaslGen::ProjectBridgeService::updateMethod( const std::string& method )
{
  return procOp.setBridgeMethod(method);
}

std::string MaslGen::ProjectBridgeService::getFileNamePrefix() const
{
  return procOp.getDefinesSynchronousOperationBridgeRequirementForNonCounterpartTerminatorR33().
                getIsDefinedWithinDomainVersionInBuildSetR113().
                getDomainKeyLetter() + "_" + procOp.getDefinesSynchronousOperationBridgeRequirementForNonCounterpartTerminatorR33().getNonCounterpartTerminatorKeyLetter() + "_";
}

std::string MaslGen::ProjectBridgeService::getDescription() const
{
  return procOp.getContractDescription();
}

bool MaslGen::ProjectBridgeService::definedInProject() const
{
  return isNative() || getMethod().size() > 0 || getTagSet().getTags().size() > 0;
}

