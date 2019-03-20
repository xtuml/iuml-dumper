// 
// Filename : Event.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Event.hh"
#include "TextUtils.hh"
#include "Parameter.hh"

#include "iumlapi/XUML_SignalParameter.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_Signal.hh"
#include "iumlapi/XUML_ExistingInstanceState.hh"
#include "iumlapi/XUML_AssignerSignal.hh"
#include "iumlapi/XUML_AssignerState.hh"
#include "iumlapi/XUML_SignalUsage.hh"
#include "iumlapi/XUML_InstanceState.hh"
#include "iumlapi/XUML_Effect.hh"

MaslGen::Event::Event ( const iUMLAPI::XUML::UMLSignal& signal )
  : signal(signal)
{
}

std::string MaslGen::Event::getEventType() const
{
  const iUMLAPI::XUML::Effect_Set& effects = 
              signal.getSubtypeSignalR128().
                     getIsUsedBySignalUsageR135().
                     getIsRespondedToInEffectR193();

  if ( signal.getSubtypeAssignerSignalR128().isValid() )
  {
    return "assigner ";
  }
  else if ( signal.getSubtypeSignalR128().
                   getIsUsedBySignalUsageR135().
                   getIsRespondedToInEffectR193().
                   getResultsInCreationInExistingInstanceStateR145().size() )
  {
    return "creation ";
  }
  else
  {
    return "";
  }
}

std::string MaslGen::Event::getModFileDeclaration( const std::string& indent ) const
{
  return wrapDescription(indent,signal.getDescription(),60) + 
          alignTabs(indent + getEventType() + "event " + makeIdentifier(signal.getSignalName()) + " " + getParameterList()) + ";\n";
}

std::string MaslGen::Event::getParameterList() const
{
  std::vector<Parameter> parameters;

  std::copy ( signal.getCarriesSupplementarySignalParameterR151().begin(), 
              signal.getCarriesSupplementarySignalParameterR151().end(), 
              std::back_inserter(parameters) );

 
  std::string paramList = "(";
  std::string prefix = "\t";

  for ( std::vector<Parameter>::const_iterator it = parameters.begin();
        it != parameters.end(); ++it )
  {
    paramList += prefix + it->getName() + " \t: in  " + it->getType();  
    prefix = ",\n\t";
  }

  paramList += ")";

  return paramList;

}
