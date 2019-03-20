// 
// Filename : DeferredInstanceService.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_DeferredInstanceService_HH
#define MaslGen_DeferredInstanceService_HH

#include <string>
#include "iumlapi/XUML_DeferredOperation.hh"
#include "iumlapi/XUML_DeferredOperationImplementation.hh"
#include "iumlapi/XUML_PolymorphicOperation.hh"
#include "iumlapi/XUML_Generalisation.hh"
#include "iumlapi/XUML_Class.hh"
#include "Service.hh"

namespace MaslGen
{
  class DeferredInstanceService : public Service
  {
    public:
      DeferredInstanceService ( const iUMLAPI::XUML::DeferredOperation& operation );
      DeferredInstanceService ( const iUMLAPI::XUML::PolymorphicOperation& operation );
      std::string getMethod() const;
      std::string getFileNamePrefix() const;
      std::string getNameQualification() const;
      std::string getCallableType() const;
      bool hasCodeFile() const;
      void updateMethod(const std::string& method);

    private:
      iUMLAPI::XUML::Generalisation generalisation;
      iUMLAPI::XUML::DeferredOperationImplementation impl;
      iUMLAPI::XUML::Class clazz;
  };
}

#endif
