// 
// Filename : Object.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Object_HH
#define MaslGen_Object_HH

#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_Generalisation.hh"

#include "ObjectInstanceService.hh"         
#include "DeferredInstanceService.hh"
#include "StateAction.hh"              
#include "AssignerStateAction.hh"              


namespace MaslGen
{
  class Object
  {
    public:
      Object ( const iUMLAPI::XUML::Class& clazz );

      void dumpToFiles() const;
      void updateFromFiles();

      std::string getModFileDeclaration( const std::string& indent ) const;
      std::string getModFileDefinition( const std::string& indent ) const;

      std::string getDescription() const { return clazz.getDescription(); }

      long getNumber() const { return clazz.getClassNumber(); }
      const iUMLAPI::XUML::Class_Set& getSupertypes() const { return clazz.getIsSubclassInGeneralisationR13().getHasSuperclassClassR12(); }

    private:
      iUMLAPI::XUML::Class clazz;
    
      std::vector<ObjectInstanceService>   oiServices;
      std::vector<DeferredInstanceService> diServices;
      std::vector<StateAction>             stateActions;
      std::vector<AssignerStateAction>     assStateActions;

  };
}


#endif
