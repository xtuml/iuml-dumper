// 
// Filename : Bridge.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Bridge_HH
#define MaslGen_Bridge_HH

#include "iumlapi/XUML_NonCounterpartTerminator.hh"

#include "BridgeEvent.hh"              
#include "BridgeService.hh"              


namespace MaslGen
{
  class Bridge
  {
    public:
      Bridge ( const iUMLAPI::XUML::NonCounterpartTerminator& nct );

      void dumpToFiles() const;
      void updateFromFiles();
      std::string getModFileDeclaration( const std::string& indent ) const;
      std::string getServiceParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const;

    private:
      iUMLAPI::XUML::NonCounterpartTerminator nct;
    
      std::vector<BridgeService> services;
      std::vector<BridgeEvent>   events;

  };
}


#endif
