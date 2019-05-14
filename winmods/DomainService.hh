// 
// Filename : DomainService.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_DomainService_HH
#define MaslGen_DomainService_HH

#include <string>
#include "iumlapi/XUML_DomainOperation.hh"
#include "Service.hh"

namespace MaslGen
{
  class DomainService : public Service
  {
    public:
      DomainService ( const iUMLAPI::XUML::DomainOperation& domainOp );
	  virtual long getOperationNumber() const;
    private:
	  iUMLAPI::XUML::DomainOperation domainOp;
  };
}

#endif
