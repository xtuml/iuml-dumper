// 
// Filename : AssignerStateTransitionTable.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "AssignerStateTransitionTable.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_AssignerState.hh"
#include "iumlapi/XUML_AssignerEffect.hh"
#include "iumlapi/XUML_AssignerSignal.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_AssociationClass.hh"
#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_AssignerSPC.hh"

MaslGen::AssignerStateTransitionTable::AssignerStateTransitionTable ( const iUMLAPI::XUML::AssignerStateModel& stateModel )
  : stateModel(stateModel)
{
}

enum EffectType { Unknown, Ignore, CantHappen, Transition };

std::string MaslGen::AssignerStateTransitionTable::getModFileDeclaration( const std::string& indent ) const
{
  std::string result = indent + "assigner transition is\n";

  const iUMLAPI::XUML::AssignerState_Set& states = stateModel.getConsistsOfAssignerStateR54();

  for ( iUMLAPI::XUML::AssignerState_Set::const_iterator state = states.begin();
        state != states.end();
        ++state )
  {
    std::string eventActions;

    std::string stateName = makeIdentifier(state->getStateName());

    eventActions+= indent + "  " + stateName + " (\t";
    std::string separator = "";

    const iUMLAPI::XUML::AssignerEffect_Set& effects = state->getIsAffectedByAssignerEffectR53();
    for ( iUMLAPI::XUML::AssignerEffect_Set::const_iterator effect = effects.begin();
          effect != effects.end();
          ++effect )
    {
      const iUMLAPI::XUML::UMLSignal& signal =
           effect->getIsAffectedByAssignerSignalR53().getSupertypeUMLSignalR128();


      eventActions+= separator + makeIdentifier(signal.getSignalName()) + " \t=> ";

      switch ( effect->getEffectType() )
      {
        case Unknown:
          eventActions+= "Unknown";
          break;

        case Ignore:
          eventActions+= "Ignore";
          break;

        case CantHappen:
          eventActions+= "Cannot_Happen";
          break;

        case Transition:
          eventActions+= makeIdentifier(effect->getCausesEntryAssignerStateR195().getStateName());
          break;
      }

      separator = ",\n\t";
    }
    eventActions += ");\n\n";
    result+= alignTabs(eventActions);
  }
  
  result += indent + "end transition;\n";

  return result;
}
