// 
// Filename : DomainService.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "DomainService.hh"
#include "TextUtils.hh"

MaslGen::DomainService::DomainService ( const iUMLAPI::XUML::DomainOperation& domainOp )
  : Service(domainOp.getSupertypeOperationR215())
{
}
