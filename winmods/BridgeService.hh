// 
// Filename : BridgeService.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_BridgeService_HH
#define MaslGen_BridgeService_HH

#include <string>
#include "iumlapi/XUML_NonCounterpartTerminatorOperation.hh"
#include "iumlapi/XUML_NonCounterpartTerminator.hh"
#include "Service.hh"

namespace MaslGen
{
  class BridgeService : public Service
  {
    public:
      BridgeService ( const iUMLAPI::XUML::NonCounterpartTerminatorOperation& nctOp );

      std::string getNameQualification() const;
      std::string getUnqualifiedName() const;
      std::string getFileNamePrefix() const;
      std::string getFileNameExtension() const;
	  long getOperationNumber() const;

    private:
      iUMLAPI::XUML::NonCounterpartTerminator nct;
	  iUMLAPI::XUML::NonCounterpartTerminatorOperation nctOp;
  };
}

#endif
