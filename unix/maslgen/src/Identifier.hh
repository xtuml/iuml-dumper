// 
// Filename : Identifier.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Identifier_HH
#define MaslGen_Identifier_HH

#include "iumlapi/XUML_Identifier.hh"

namespace MaslGen
{
  class Identifier
  {
    public:
      Identifier ( const iUMLAPI::XUML::Identifier& identifier );

      std::string getModFileDeclaration( const std::string& indent ) const;

    private:
      iUMLAPI::XUML::Identifier identifier;
    
  };
}


#endif
