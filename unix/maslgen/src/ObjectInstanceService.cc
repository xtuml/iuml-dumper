// 
// Filename : ObjectInstanceService.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "ObjectInstanceService.hh"
#include "TextUtils.hh"

MaslGen::ObjectInstanceService::ObjectInstanceService ( const iUMLAPI::XUML::ClassOperation& classOp )
  : Service(classOp.getSupertypeOperationR215()),
    clazz(classOp.getBelongsToClassR219()),
    instanceBased(classOp.getIsInstanceBased())
{
}

std::string MaslGen::ObjectInstanceService::getFileNamePrefix() const
{
  return makeIdentifier(clazz.getClassKeyLetter()==""?clazz.getClassName():clazz.getClassKeyLetter()) + "_";
}

std::string MaslGen::ObjectInstanceService::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(clazz.getClassName()) + ".";
}

std::string MaslGen::ObjectInstanceService::getCallableType() const
{
  return std::string(isPublic()?"public":"private") + " " +
                      (instanceBased?"instance ":"") +
                      (isFunction()?"function":"service");
}
