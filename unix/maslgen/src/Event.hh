// 
// Filename : Event.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Event_HH
#define MaslGen_Event_HH

#include <string>
#include "iumlapi/XUML_UMLSignal.hh"
#include "Callable.hh"

namespace MaslGen
{
  class Event
  {
    public:
      Event ( const iUMLAPI::XUML::UMLSignal& signal );

      std::string getModFileDeclaration( const std::string& indent) const;
      std::string getParameterList() const;
      std::string getEventType() const;

    private:
      iUMLAPI::XUML::UMLSignal signal;
  };
}

#endif
