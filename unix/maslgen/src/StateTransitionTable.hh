// 
// Filename : StateTransitionTable.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_StateTransitionTable_HH
#define MaslGen_StateTransitionTable_HH

#include "iumlapi/XUML_InstanceStateModel.hh"

namespace MaslGen
{
  class StateTransitionTable
  {
    public:
      StateTransitionTable ( const iUMLAPI::XUML::InstanceStateModel& stateModel );

      std::string getModFileDeclaration( const std::string& indent ) const;

    private:
      iUMLAPI::XUML::InstanceStateModel stateModel;
    
  };
}


#endif
