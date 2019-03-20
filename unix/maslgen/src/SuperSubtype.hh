// 
// Filename : SuperSubtype.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_SuperSubtype_HH
#define MaslGen_SuperSubtype_HH

#include "iumlapi/XUML_Generalisation.hh"

namespace MaslGen
{
  class SuperSubtype
  {
    public:
      SuperSubtype ( const iUMLAPI::XUML::Generalisation& generalisation );

      std::string getModFileDeclaration( const std::string& indent ) const;

    private:
      iUMLAPI::XUML::Generalisation generalisation;
    
  };
}


#endif
