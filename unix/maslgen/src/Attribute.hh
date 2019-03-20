// 
// Filename : Attribute.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Attribute_HH
#define MaslGen_Attribute_HH

#include "iumlapi/XUML_Attribute.hh"
#include "TagSet.hh"

namespace MaslGen
{
  class Attribute
  {
    public:
      Attribute ( const iUMLAPI::XUML::Attribute& attribute );

      std::string getModFileDeclaration( const std::string& indent ) const;
      std::string getPragmas() const;

    private:
      iUMLAPI::XUML::Attribute attribute;
      TagSet tags;
    
    private:
      static const std::string& uniqueTag();
  };
}


#endif
