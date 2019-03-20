// 
// Filename : EventAction.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "EventAction.hh"
#include "TextUtils.hh"
#include "Parameter.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_SignalParameter.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"

MaslGen::EventAction::EventAction ( const iUMLAPI::XUML::UMLSignal& signal )
  : signal(signal)
{
}

std::string MaslGen::EventAction::getUnqualifiedName() const
{
  return makeIdentifier(signal.getSignalName());
}

std::string MaslGen::EventAction::getDomainName() const
{
  return iUMLAPI::getDomainKeyLetter(signal.getIsDirectedAtSignalProducerConsumerR166().getIsDefinedInAnalysisAreaR237());
}

std::string MaslGen::EventAction::getFileNameExtension() const
{
  return "al";
}

std::string MaslGen::EventAction::getDescription() const
{
  return signal.getDescription();
}

std::string MaslGen::EventAction::getParameterList() const
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
