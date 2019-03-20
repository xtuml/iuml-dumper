// 
// Filename : AssignerStateAction.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "AssignerStateAction.hh"
#include "TextUtils.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AssignerStateModel.hh"
#include "iumlapi/XUML_AssignerEffect.hh"
#include "iumlapi/XUML_AssignerSignal.hh"
#include "iumlapi/XUML_AssociationClass.hh"
#include "iumlapi/XUML_Signal.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

#include <iostream>

iUMLAPI::XUML::UMLSignal getFirstSignal( const iUMLAPI::XUML::AssignerState& state )
{
  const iUMLAPI::XUML::UMLSignal_Set& signals = state.getIsEnteredAsAResultOfAssignerEffectR195().
                    getIsAffectedByAssignerSignalR53().getSupertypeUMLSignalR128();
  if ( signals.size() > 0 )
  {
    return signals[0];
  }
  else
  {
    std::cerr << "Error: No Assigner Signals for " << state.getStateName() << std::endl;
    return iUMLAPI::XUML::UMLSignal(iUMLAPI::Entity());
  }
}


MaslGen::AssignerStateAction::AssignerStateAction ( const iUMLAPI::XUML::AssignerState& state )
  : EventAction(getFirstSignal(state)),
    state(state),
    clazz(state.getIsAStateOfAssignerStateModelR54().
                getModelsTheAssignmentDynamicsOfAssociationClassR60().
                getSupertypeClassR5())
{
}

std::string MaslGen::AssignerStateAction::getDomainName() const
{
    return iUMLAPI::getDomainKeyLetter(clazz.getIsDefinedInAnalysisAreaR224());
}

std::string MaslGen::AssignerStateAction::getDescription() const
{
  return state.getDescription();
}

std::string MaslGen::AssignerStateAction::getCallableType() const
{
  return std::string("assigner ") + (state.getIsStartState()?"start ":"") + "state";
}

std::string MaslGen::AssignerStateAction::getNameQualification() const
{
  return getDomainName() + "::" + makeIdentifier(clazz.getClassName()) + ".";
}

std::string MaslGen::AssignerStateAction::getUnqualifiedName() const
{
  return makeIdentifier(state.getStateName());
}

std::string MaslGen::AssignerStateAction::getFileNamePrefix() const
{
  return makeIdentifier(clazz.getClassKeyLetter()==""?clazz.getClassName():clazz.getClassKeyLetter()) + "-A_";
}

std::string MaslGen::AssignerStateAction::getMethod() const
{
  return state.getMethod();
}

void MaslGen::AssignerStateAction::updateMethod( const std::string& method )
{
  return state.setMethod(method);
}


