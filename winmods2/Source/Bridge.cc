// 
// Filename : Bridge.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Bridge.hh"
#include "TextUtils.hh"
#include "TagSet.hh"

#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"

#include <iterator>

#include "boost/lexical_cast.hpp"

MaslGen::Bridge::Bridge ( const iUMLAPI::XUML::NonCounterpartTerminator& nct_ )
  : nct(nct_)
{
  // Bridge Services
  const iUMLAPI::XUML::NonCounterpartTerminatorOperation_Set& ncto = 
              nct.getProvidesNonCounterpartTerminatorOperationR218();
  std::copy(ncto.begin(), ncto.end(), back_inserter(services)); 

  // Bridge Events
  const iUMLAPI::XUML::TerminatorSignal_Set& ts = 
              nct.getSupertypeSignalProducerConsumerR157().
                  getIsTargetOfUMLSignalR166().
                  getSubtypeTerminatorSignalR128();
  std::copy(ts.begin(), ts.end(), back_inserter(events));
}

void MaslGen::Bridge::dumpToFiles() const
{
  std::for_each ( services.begin(), services.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( events.begin(), events.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
}

void MaslGen::Bridge::updateFromFiles()
{
  std::for_each ( services.begin(), services.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( events.begin(), events.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
}

std::string MaslGen::Bridge::getServiceParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const
{
  std::string result;

  for ( std::vector<BridgeEvent>::const_iterator it = events.begin();
        it != events.end();
        ++it )
  {
    result += it->getParameterTypeDefs ( "  ", alreadyDefined );
  }


  for ( std::vector<BridgeService>::const_iterator it = services.begin();
        it != services.end();
        ++it )
  {
    result += it->getParameterTypeDefs ( "  ", alreadyDefined );
  }

  return result;

}


std::string MaslGen::Bridge::getModFileDeclaration(const std::string& indent) const
{
  std::string result;

  result+= wrapDescription(indent,nct.getDescription(),1000) + 
           indent + "terminator " + makeIdentifier(nct.getName()) + " is\n";
  for ( std::vector<BridgeEvent>::const_iterator it = events.begin();
        it != events.end();
        ++it )
  {
    result += it->getModFileDeclaration("  " + indent) + "\n";
  }

  for ( std::vector<BridgeService>::const_iterator it = services.begin();
        it != services.end();
        ++it )
  {
    result += it->getModFileDeclaration("  " + indent) /* + "\n" */;

	result += indent + "  " + "pragma terminator_operation_number(" + boost::lexical_cast<std::string>(it->getOperationNumber()) + ");\n\n";

	
	
  }
  result+= indent + "end terminator;\n";
  if ( nct.getSupertypeSignalProducerConsumerR157().getKeyLetter() != "" )
  {
    result+= indent + "pragma key_letter (\"" + makeIdentifier(nct.getSupertypeSignalProducerConsumerR157().getKeyLetter()) + "\");\n";
  }

  TagSet tags(nct.getTAGS_TaggedItem());
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
        it != tags.getTags().end();
        ++it )
  {
    result+= indent + "pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");\n";
  }
  result+= "\n";

  return result;
}
