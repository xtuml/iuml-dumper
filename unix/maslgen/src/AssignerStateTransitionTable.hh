// 
// Filename : AssignerStateTransitionTable.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_AssignerStateTransitionTable_HH
#define MaslGen_AssignerStateTransitionTable_HH

#include "iumlapi/XUML_AssignerStateModel.hh"

namespace MaslGen
{
  class AssignerStateTransitionTable
  {
    public:
      AssignerStateTransitionTable ( const iUMLAPI::XUML::AssignerStateModel& stateModel );

      std::string getModFileDeclaration( const std::string& indent ) const;

    private:
      iUMLAPI::XUML::AssignerStateModel stateModel;
    
  };
}


#endif
