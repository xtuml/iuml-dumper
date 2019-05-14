// 
// Filename : StateAction.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_StateAction_HH
#define MaslGen_StateAction_HH

#include <string>
#include "iumlapi/XUML_ExistingInstanceState.hh"
#include "iumlapi/XUML_Class.hh"
#include "EventAction.hh"

namespace MaslGen
{
  class StateAction : public EventAction
  {
    public:
      StateAction ( const iUMLAPI::XUML::ExistingInstanceState& state );

      virtual std::string getCallableType() const;
      virtual std::string getNameQualification() const;
      virtual std::string getUnqualifiedName() const;
      virtual std::string getFileNamePrefix() const;
      virtual std::string getMethod() const;
      virtual std::string getDomainName() const;
      virtual std::string getDescription() const;
	  virtual long getStateNumber() const;
      virtual void updateMethod(const std::string& method);

    private:
      iUMLAPI::XUML::ExistingInstanceState state;
      iUMLAPI::XUML::Class clazz;
  };
}

#endif
