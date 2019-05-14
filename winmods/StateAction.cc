// 
// Filename : StateAction.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "StateAction.hh"
#include "TextUtils.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_InstanceState.hh"
#include "iumlapi/XUML_Effect.hh"
#include "iumlapi/XUML_InstanceStateModel.hh"
#include "iumlapi/XUML_Signal.hh"
#include "iumlapi/XUML_SignalUsage.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

iUMLAPI::XUML::UMLSignal getFirstSignal( const iUMLAPI::XUML::ExistingInstanceState& state )
{
  if ( state.getIsCreation() )
  {
    return state.getIsEnteredAsAResultOfEffectR145().getRespondsToSignalUsageR193().getUsesSignalR135().getSupertypeUMLSignalR128();
  }
  else
  {
    const iUMLAPI::XUML::UMLSignal_Set& signals = state.getIsEnteredAsAResultOfEffectR196()
                                                       .getRespondsToSignalUsageR193()
                                                       .getUsesSignalR135()
                                                       .getSupertypeUMLSignalR128();
    if ( signals.size() > 0 )
    {
      return signals[0];
    }
    else
    {
      return iUMLAPI::XUML::UMLSignal(iUMLAPI::Entity());
    }
  }

}



MaslGen::StateAction::StateAction ( const iUMLAPI::XUML::ExistingInstanceState& state )
  : EventAction(getFirstSignal(state)),
    state(state),
    clazz(state.getSupertypeInstanceStateR132().
                getBelongsToInstanceStateModelR133().
                getModelsTheInstanceDynamicBehaviourOfClassR61())
{
}

std::string MaslGen::StateAction::getDomainName() const
{
  return iUMLAPI::getDomainKeyLetter(clazz.getIsDefinedInAnalysisAreaR224());
}

std::string MaslGen::StateAction::getDescription() const
{
  return state.getDescription();
}

std::string MaslGen::StateAction::getCallableType() const
{
  return std::string(state.getIsCreation()?"creation ":"") + 
         (state.getIsTerminal()?"terminal ":"") + 
         "state";
}

std::string MaslGen::StateAction::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(clazz.getClassName()) + ".";
}

std::string MaslGen::StateAction::getUnqualifiedName() const
{
  return makeIdentifier(state.getStateName());
}

std::string MaslGen::StateAction::getFileNamePrefix() const
{
  return makeIdentifier(clazz.getClassKeyLetter()==""?clazz.getClassName():clazz.getClassKeyLetter()) + "_";
}

std::string MaslGen::StateAction::getMethod() const
{
  return state.getMethod();
}

long MaslGen::StateAction::getStateNumber() const
{
	return state.getSupertypeInstanceStateR132().getStateNumber();
}

void MaslGen::StateAction::updateMethod(const std::string& method)
{
  state.setMethod(method);
}
