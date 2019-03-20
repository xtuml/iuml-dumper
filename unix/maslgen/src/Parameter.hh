// 
// Filename : Parameter.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Parameter_HH
#define MaslGen_Parameter_HH

#include "iumlapi/XUML_OperationParameter.hh"
#include "iumlapi/XUML_SignalParameter.hh"

namespace MaslGen
{
  class Parameter
  {
    public:
      Parameter ( const iUMLAPI::XUML::OperationParameter& param );
      Parameter ( const iUMLAPI::XUML::SignalParameter& param );

      const std::string& getName() const { return name; }
      const std::string& getType() const { return type; }

    private:
      std::string name;
      std::string type;
  };

}
#endif
