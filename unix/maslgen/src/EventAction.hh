// 
// Filename : EventAction.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_EventAction_HH
#define MaslGen_EventAction_HH

#include <string>
#include "iumlapi/XUML_UMLSignal.hh"
#include "Callable.hh"

namespace MaslGen
{
  class EventAction : public Callable
  {
    public:
      EventAction ( const iUMLAPI::XUML::UMLSignal& signal );

      virtual ~EventAction() {}

      virtual std::string getFileNameExtension() const;
      virtual std::string getDescription() const;

      virtual std::string getUnqualifiedName() const;
      virtual std::string getDomainName() const;
      virtual std::string getParameterList() const;

    private:
      iUMLAPI::XUML::UMLSignal signal;
  };
}

#endif
