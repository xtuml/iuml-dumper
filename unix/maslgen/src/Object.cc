// 
// Filename : Object.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Object.hh"
#include "TextUtils.hh"
#include "Attribute.hh"
#include "StateTransitionTable.hh"
#include "AssignerStateTransitionTable.hh"
#include "Event.hh"
#include "Identifier.hh"

#include "iumlapi/TAGS_TaggedItem.hh"
#include "iumlapi/XUML_Class.hh"
#include "iumlapi/XUML_DeferredOperation.hh"
#include "iumlapi/XUML_InstanceStateModel.hh"
#include "iumlapi/XUML_AssociationClass.hh"
#include "iumlapi/XUML_InstanceState.hh"
#include "iumlapi/XUML_AssignerStateModel.hh"
#include "iumlapi/XUML_Attribute.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_InstanceSPC.hh"
#include "iumlapi/XUML_AssignerSPC.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_SignalTransmission.hh"

#include "boost/lexical_cast.hpp"

MaslGen::Object::Object ( const iUMLAPI::XUML::Class& clazz_ )
  : clazz(clazz_)
{
  // Object Instance Services
  const iUMLAPI::XUML::ClassOperation_Set& ois = clazz.getHasClassOperationR219();
  std::copy(ois.begin(), ois.end(), back_inserter(oiServices)); 

  // Deferred Instance Services
  const iUMLAPI::XUML::DeferredOperation_Set& dis =
              clazz.getIsAtBranchWhichIsTheLocationOfDeferredOperationR245();
  std::copy(dis.begin(), dis.end(), back_inserter(diServices)); 

  // State Actions
  const iUMLAPI::XUML::ExistingInstanceState_Set& sa = 
              clazz.getHasInstanceDynamicsModelledByInstanceStateModelR61().
                     getContainsInstanceStateR133().
                     getSubtypeExistingInstanceStateR132();
  std::copy(sa.begin(), sa.end(), back_inserter(stateActions)); 

  // Assigner State Actions
  const iUMLAPI::XUML::AssignerState_Set& asa = 
              clazz.getSubtypeAssociationClassR5().
                     getHasAssignmentDynamicsModelledByAssignerStateModelR60().
                     getConsistsOfAssignerStateR54();
  std::copy(asa.begin(), asa.end(), back_inserter(assStateActions)); 

}

void MaslGen::Object::dumpToFiles() const
{
  std::for_each ( oiServices.begin(), oiServices.end(), std::mem_fun_ref(CodeFile::dumpToFile) );
  std::for_each ( diServices.begin(), diServices.end(), std::mem_fun_ref(CodeFile::dumpToFile) );
  std::for_each ( stateActions.begin(), stateActions.end(), std::mem_fun_ref(CodeFile::dumpToFile) );
  std::for_each ( assStateActions.begin(), assStateActions.end(), std::mem_fun_ref(CodeFile::dumpToFile) );
}

void MaslGen::Object::updateFromFiles()
{
  std::for_each ( oiServices.begin(), oiServices.end(), std::mem_fun_ref(CodeFile::updateFromFile) );
  std::for_each ( diServices.begin(), diServices.end(), std::mem_fun_ref(CodeFile::updateFromFile) );
  std::for_each ( stateActions.begin(), stateActions.end(), std::mem_fun_ref(CodeFile::updateFromFile) );
  std::for_each ( assStateActions.begin(), assStateActions.end(), std::mem_fun_ref(CodeFile::updateFromFile) );
}

std::string MaslGen::Object::getModFileDefinition(const std::string& indent) const
{
  std::string result = wrapDescription(indent,clazz.getDescription(),60) + 
                       indent + "object " + makeIdentifier(clazz.getClassName()) + " is\n\n";

  // Attributes
  const iUMLAPI::XUML::Attribute_Set& attributes = clazz.getIsCharacterisedByAttributeR9();

  std::string attDefs;
  for ( iUMLAPI::XUML::Attribute_Set::const_iterator it = attributes.begin();
        it != attributes.end();
        ++it )
  {
    attDefs += Attribute(*it).getModFileDeclaration(indent + "  ") + "\n";
  }
  result+= alignTabs(attDefs);

  // Identifiers
  const iUMLAPI::XUML::Identifier_Set& identifiers = clazz.getIsIdentifiedByIdentifierR14();

  for ( iUMLAPI::XUML::Identifier_Set::const_iterator it = identifiers.begin();
        it != identifiers.end();
        ++it )
  {
    result+= Identifier(*it).getModFileDeclaration(indent + "  ") + "\n";
  }

  // Polymorphic Instance Services
  for ( iUMLAPI::XUML::PolymorphicOperation_Set::const_iterator it = clazz.getHasPolymorphicOperationR17().begin();
        it != clazz.getHasPolymorphicOperationR17().end();
        ++it )
  {
    result += DeferredInstanceService(*it).getModFileDeclaration(indent + "  ") + "\n";
  }

  // Instance Services
  for ( std::vector<ObjectInstanceService>::const_iterator it = oiServices.begin();
        it != oiServices.end();
        ++it )
  {
    result += it->getModFileDeclaration(indent + "  ") + "\n";
  }

  // Deferred Instance Services
  for ( std::vector<DeferredInstanceService>::const_iterator it = diServices.begin();
        it != diServices.end();
        ++it )
  {
    result += it->getModFileDeclaration(indent + "  ") + "\n";
  }

  // State Actions
  for ( std::vector<StateAction>::const_iterator it = stateActions.begin();
        it != stateActions.end();
        ++it )
  {
    result += it->getModFileDeclaration(indent + "  ") + "\n";
  }

  // Events
  const iUMLAPI::XUML::UMLSignal_Set& events = 
                    clazz.getHasRoleInstanceSPCR168().
                          getSupertypeSignalProducerConsumerR157().                       
                          getIsTargetOfUMLSignalR166();

  for ( iUMLAPI::XUML::UMLSignal_Set::const_iterator it = events.begin();
        it != events.end();
        ++it )
  {
    result += Event(*it).getModFileDeclaration(indent + "  ") + "\n";
  }

  // State Transition Table
  if ( clazz.getHasInstanceDynamicsModelledByInstanceStateModelR61().isValid() )
  {
    result+= StateTransitionTable(clazz.getHasInstanceDynamicsModelledByInstanceStateModelR61()).
                      getModFileDeclaration(indent + "  ") + "\n";
  }

  // Assigner State Actions
  for ( std::vector<AssignerStateAction>::const_iterator it = assStateActions.begin();
        it != assStateActions.end();
        ++it )
  {
    result += it->getModFileDeclaration(indent + "  ") + "\n";
  }

  // Assigner Events
  const iUMLAPI::XUML::UMLSignal_Set& assignerEvents = 
                    clazz.getSubtypeAssociationClassR5().
                          getHasRoleAssignerSPCR169().
                          getSupertypeSignalProducerConsumerR157().                       
                          getIsTargetOfUMLSignalR166();

  for ( iUMLAPI::XUML::UMLSignal_Set::const_iterator it = assignerEvents.begin();
        it != assignerEvents.end();
        ++it )
  {
    result += Event(*it).getModFileDeclaration(indent + "  ") + "\n";
  }

  // Assigner State Transition Table
  if ( clazz.getSubtypeAssociationClassR5().getHasAssignmentDynamicsModelledByAssignerStateModelR60().isValid() )
  {
    result+= AssignerStateTransitionTable(clazz.getSubtypeAssociationClassR5().
                                                       getHasAssignmentDynamicsModelledByAssignerStateModelR60()).
                      getModFileDeclaration(indent + "  ") + "\n";
  }

  result += indent + "end object;\n";
  result += indent + "pragma id (" + boost::lexical_cast<std::string>(clazz.getClassNumber()) + ");\n";
  if ( clazz.getClassKeyLetter() != "" )
  {
    result += indent + "pragma key_letter (\"" + makeIdentifier(clazz.getClassKeyLetter()) + "\");\n";
  }

  TagSet tags(clazz.getTAGS_TaggedItem());
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
        it != tags.getTags().end();
        ++it )
  {
    result+= indent + "pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");\n";
  }

  return result;
}

std::string MaslGen::Object::getModFileDeclaration(const std::string& indent) const
{
  return indent + "object " + makeIdentifier(clazz.getClassName()) + ";\n";
}
