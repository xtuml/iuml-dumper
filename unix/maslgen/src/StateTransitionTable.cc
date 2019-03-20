// 
// Filename : StateTransitionTable.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "StateTransitionTable.hh"
#include "TextUtils.hh"

#include "iumlapi/XUML_InstanceState.hh"
#include "iumlapi/XUML_ExistingInstanceState.hh"
#include "iumlapi/XUML_NonexistentState.hh"
#include "iumlapi/XUML_Effect.hh"
#include "iumlapi/XUML_Signal.hh"
#include "iumlapi/XUML_SignalUsage.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_InstanceSPC.hh"

MaslGen::StateTransitionTable::StateTransitionTable ( const iUMLAPI::XUML::InstanceStateModel& stateModel )
  : stateModel(stateModel)
{
}

enum EffectType { Unknown, Ignore, CantHappen, Transition, Meaningless, Creation, Hold, NTA };

std::string MaslGen::StateTransitionTable::getModFileDeclaration( const std::string& indent ) const
{
  std::string result = indent + "transition is\n";

  const iUMLAPI::XUML::InstanceState_Set& states = stateModel.getContainsInstanceStateR133();

  const iUMLAPI::XUML::Class& targetClass = 
          stateModel.getModelsTheInstanceDynamicBehaviourOfClassR61();


  for ( iUMLAPI::XUML::InstanceState_Set::const_iterator state = states.begin();
        state != states.end();
        ++state )
  {
    std::string eventActions;

    bool isFromNonExistentState = state->getSubtypeNonexistentStateR132().isValid();

    std::string stateName = isFromNonExistentState ?
                              std::string("Non_Existent") :
                              makeIdentifier(state->getSubtypeExistingInstanceStateR132().getStateName());


    eventActions+= indent + "  " + stateName + " (\t";
    std::string separator = "";

    const iUMLAPI::XUML::Effect_Set& effects = state->getRespondsToEffectR193();
    for ( iUMLAPI::XUML::Effect_Set::const_iterator effect = effects.begin();
          effect != effects.end();
          ++effect )
    {
      const iUMLAPI::XUML::UMLSignal& signal =
           effect->getRespondsToSignalUsageR193().getUsesSignalR135().getSupertypeUMLSignalR128();

       bool isCreationEvent =  signal.getSubtypeSignalR128()
                                     .getIsUsedBySignalUsageR135()
                                     .getIsRespondedToInEffectR193()
                                     .getResultsInCreationInExistingInstanceStateR145()
                                     .size();
                                    
      const iUMLAPI::XUML::Class& sourceClass = 
              signal.getIsDirectedAtSignalProducerConsumerR166().
                     getSubtypeInstanceSPCR157().
                     getPerformsSignalRoleForClassR168();

      std::string eventObject;
      if ( !(targetClass == sourceClass) )
      {
        eventObject = makeIdentifier(sourceClass.getClassName()) + ".";
      }

      eventActions+= separator + eventObject + makeIdentifier(signal.getSignalName()) + " \t=> ";

      long effectType = isCreationEvent == isFromNonExistentState ? effect->getEffectType() : CantHappen;
      
      switch ( effectType )
      {
        case Ignore:
          eventActions+= "Ignore";
          break;

        case Meaningless:
        case Unknown:
        case CantHappen:
        case NTA:
        case Hold:
          eventActions+= "Cannot_Happen";
          break;

        case Transition:
          eventActions+= makeIdentifier(effect->getCausesEntryToExistingInstanceStateR196().getStateName());
          break;

        case Creation:
          eventActions+= makeIdentifier(effect->getResultsInCreationInExistingInstanceStateR145().getStateName());
          break;


      }

      separator = ",\n\t";
    }
    eventActions += ");\n";
    result+= eventActions;
  }
  
  result += indent + "end transition;\n";

  return alignTabs(result);
}
