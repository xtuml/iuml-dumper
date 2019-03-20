// 
// Filename : Relationship.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Relationship_HH
#define MaslGen_Relationship_HH

#include "iumlapi/XUML_UMLAssociation.hh"

namespace MaslGen
{
  class Relationship
  {
    public:
      Relationship ( const iUMLAPI::XUML::UMLAssociation& association );

      std::string getModFileDeclaration( const std::string& indent ) const;

    private:
      iUMLAPI::XUML::UMLAssociation association;
    
  };
}


#endif
