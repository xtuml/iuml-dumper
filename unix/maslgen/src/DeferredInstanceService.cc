// 
// Filename : DeferredInstanceService.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "DeferredInstanceService.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_DeferredOperation.hh"
#include "iumlapi/XUML_FurtherOperationDeferral.hh"
#include "iumlapi/XUML_PolymorphicOperation.hh"
#include "iumlapi/XUML_Generalisation.hh"

#include "boost/lexical_cast.hpp"

iUMLAPI::XUML::Operation findBaseOperation ( const iUMLAPI::XUML::DeferredOperation& operation )
{
  iUMLAPI::XUML::DeferredOperation deferredOp = operation;
  iUMLAPI::XUML::DeferredOperation deferredFrom = deferredOp.getImplementsFurtherOperationDeferralR244().getSupertypeDeferredOperationR241();

  while ( deferredFrom.isValid() )
  {
    deferredOp = deferredFrom;
    deferredFrom = deferredOp.getImplementsFurtherOperationDeferralR244().getSupertypeDeferredOperationR241();
  }

  return deferredOp.getImplementsPolymorphicOperationR240().getSupertypeOperationR215();
  
}

MaslGen::DeferredInstanceService::DeferredInstanceService ( const iUMLAPI::XUML::DeferredOperation& operation )
  : Service(findBaseOperation(operation)),
    generalisation(operation.getSubtypeFurtherOperationDeferralR241().getIsFurtherDeferredDownGeneralisationR243()),
    impl(operation.getSubtypeDeferredOperationImplementationR241()),
    clazz(operation.getIsLocatedAtBranchWithClassR245())
{
}

MaslGen::DeferredInstanceService::DeferredInstanceService ( const iUMLAPI::XUML::PolymorphicOperation& operation )
  : Service(operation.getSupertypeOperationR215()),
    generalisation(operation.getIsDeferredDownGeneralisationR242()),
    impl(),
    clazz(operation.getBelongsToClassR17())
{
}

bool MaslGen::DeferredInstanceService::hasCodeFile() const
{
  return impl.isValid();
}

std::string MaslGen::DeferredInstanceService::getMethod() const
{
  return impl.getMethod();
}

void MaslGen::DeferredInstanceService::updateMethod( const std::string& method )
{
  return impl.setMethod(method);
}

std::string MaslGen::DeferredInstanceService::getFileNamePrefix() const
{
  return makeIdentifier(clazz.getClassKeyLetter()==""?clazz.getClassName():clazz.getClassKeyLetter()) + "_";
}

std::string MaslGen::DeferredInstanceService::getCallableType() const
{
  std::string deferral;
  if ( !impl.isValid() )
  {
    deferral = "deferred (R" + boost::lexical_cast<std::string>(generalisation.getRelationshipNumber()) + ") "; 
  }
  return std::string(isPublic()?"public":"private") + " instance " + deferral + (isFunction()?"function":"service");
}

std::string MaslGen::DeferredInstanceService::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(clazz.getClassName()) + ".";
}


