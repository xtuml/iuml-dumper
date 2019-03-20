// 
// Filename : AssignerStateAction.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_AssignerStateAction_HH
#define MaslGen_AssignerStateAction_HH

#include <string>
#include "iumlapi/XUML_AssignerState.hh"
#include "iumlapi/XUML_Class.hh"
#include "EventAction.hh"

namespace MaslGen
{
  class AssignerStateAction : public EventAction
  {
    public:
      AssignerStateAction ( const iUMLAPI::XUML::AssignerState& state );

      virtual std::string getCallableType() const;
      virtual std::string getNameQualification() const;
      virtual std::string getUnqualifiedName() const;
      virtual std::string getFileNamePrefix() const;
      virtual std::string getMethod() const;
      virtual std::string getDomainName() const;
      virtual std::string getDescription() const;
      virtual void updateMethod(const std::string& method);

    private:
      iUMLAPI::XUML::AssignerState state;
      iUMLAPI::XUML::Class clazz;
  };
}

#endif
