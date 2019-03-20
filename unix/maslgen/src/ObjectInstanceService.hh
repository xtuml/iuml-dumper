// 
// Filename : ObjectInstanceService.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_ObjectInstanceService_HH
#define MaslGen_ObjectInstanceService_HH

#include <string>
#include "iumlapi/XUML_ClassOperation.hh"
#include "iumlapi/XUML_Class.hh"
#include "Service.hh"

namespace MaslGen
{
  class ObjectInstanceService : public Service
  {
    public:
      ObjectInstanceService ( const iUMLAPI::XUML::ClassOperation& classOp );

      std::string getFileNamePrefix() const;
      std::string getNameQualification() const;
      std::string getCallableType() const;

    private:
      iUMLAPI::XUML::Class clazz;
      bool instanceBased;
  };
}

#endif
