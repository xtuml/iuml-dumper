// 
// Filename : Domain.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Domain.hh"
#include "Relationship.hh"
#include "SuperSubtype.hh"
#include "DataType.hh"
#include "DumpOptions.hh"
#include "TextUtils.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_DeferredOperation.hh"
#include "iumlapi/XUML_InstanceStateModel.hh"
#include "iumlapi/XUML_AssociationClass.hh"
#include "iumlapi/XUML_InstanceState.hh"
#include "iumlapi/XUML_AssignerStateModel.hh"
#include "iumlapi/XUML_UMLAssociation.hh"
#include "iumlapi/XUML_Generalisation.hh"
#include "iumlapi/XUML_UserDefinedDataType.hh"
#include "iumlapi/ORG_Domain.hh"

#include <algorithm>

#include "boost/bind.hpp"
#include "boost/lexical_cast.hpp"

MaslGen::Domain::Domain ( const iUMLAPI::ORG::DomainVersion& domainVersion )
  : analysis(iUMLAPI::getAnalysisArea(domainVersion)),
    cacheInitialised(false),
    interfaceSectionInitialised(false)
{
}

void MaslGen::Domain::initCache() const
{
  if ( !cacheInitialised )
  {
    cacheInitialised = true;
    // Domain Services
    std::copy(analysis.getContainsDomainOperationR222().begin(), 
              analysis.getContainsDomainOperationR222().end(),
              back_inserter(domainServices)); 

    // Supplementary Code Blocks
    std::copy(analysis.getContainsSupplementaryCodeBlockR999().begin(), 
              analysis.getContainsSupplementaryCodeBlockR999().end(),
              back_inserter(scbs)); 

    // Scenarios
    std::copy(analysis.getContainsInitialisationSegmentR233().begin(), 
              analysis.getContainsInitialisationSegmentR233().end(),
              back_inserter(scenarios)); 

    // Externals
    std::copy(analysis.getContainsTestMethodR227().begin(), 
              analysis.getContainsTestMethodR227().end(),
              back_inserter(externals)); 

    // Bridges
    std::copy(analysis.getContainsNonCounterpartTerminatorR223().begin(), 
              analysis.getContainsNonCounterpartTerminatorR223().end(),
              back_inserter(bridges)); 

    // Objects
    std::copy(analysis.getContainsClassR224().begin(), 
              analysis.getContainsClassR224().end(),
              back_inserter(objects)); 

    // Types
    std::copy(analysis.getContainsDataTypeR221().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().begin(),
              analysis.getContainsDataTypeR221().getSubtypeUserDefinedDataTypeR73().getSupertypeDataTypeR73().end(),
              std::back_inserter(types));
  }
}

void MaslGen::Domain::dumpToFiles() const
{
  initCache();

  std::for_each ( domainServices.begin(), domainServices.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( scbs.begin(), scbs.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( externals.begin(), externals.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( scenarios.begin(), scenarios.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( objects.begin(), objects.end(), std::mem_fun_ref(&Object::dumpToFiles) );
  std::for_each ( bridges.begin(), bridges.end(), std::mem_fun_ref(&Bridge::dumpToFiles) );

  dumpToFile();
  Interface(*this).dumpToFile();
}

void MaslGen::Domain::updateFromFiles()
{
  initCache();

  std::for_each ( domainServices.begin(), domainServices.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( scbs.begin(), scbs.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( externals.begin(), externals.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( scenarios.begin(), scenarios.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( objects.begin(), objects.end(), std::mem_fun_ref(&Object::updateFromFiles) );
  std::for_each ( bridges.begin(), bridges.end(), std::mem_fun_ref(&Bridge::updateFromFiles) );
}


std::string MaslGen::Domain::getFileName() const
{
  return getKeyLetter() + ".mod";
}

std::string MaslGen::Domain::getCopyrightPrefix() const
{
  return "//";
}

std::string MaslGen::Domain::getDescription() const
{
  return getDomainVersion(analysis).getIsAViewOfDomainR4().getMissionStatement();
}

const std::string& MaslGen::Domain::getKeyLetter() const
{
  return getDomainKeyLetter(analysis);
}

std::string MaslGen::Domain::getDeclaration() const
{
  return "domain " + getKeyLetter() + " is\n";
}

std::string MaslGen::Domain::getSignOff() const
{
  return "end domain;\npragma number (" + boost::lexical_cast<std::string>(iUMLAPI::getDomainNumber(analysis)) + ");\n";
}

std::string MaslGen::Domain::getFileContents() const
{
  std::string modFile = getCopyrightNotice() + wrapDescription("",getDescription(),60) + getDeclaration();

  // Object predeclarations
  for ( std::vector<Object>::const_iterator it = objects.begin();
        it != objects.end();
        ++it )
  {
    modFile += it->getModFileDeclaration("  ");
  }

  if ( objects.size() ) modFile += "\n";

  // Public Data Types
  std::set<std::string> alreadyDumped;
  for ( std::vector<DataType>::const_iterator it = types.begin();
        it != types.end();
        ++it )
  {
    if ( it->isPublic() )
    {
      modFile += it->getModFileFullDefinition("  ",alreadyDumped);
    }
  }

  // Domain Service Parameter Data Types - need to do this separately so that they can be forced public
  for ( std::vector<DomainService>::const_iterator it = domainServices.begin();
        it != domainServices.end();
        ++it )
  {
    if ( it->isPublic() )
    {
      modFile += it->getParameterTypeDefs ( "  ", alreadyDumped );
    }
  }

  // Terminator Service Parameter Data Types - need to do this separately so that they can be forced public
  for ( std::vector<Bridge>::const_iterator it = bridges.begin();
        it != bridges.end();
        ++it )
  {
    modFile += it->getServiceParameterTypeDefs ( "  ", alreadyDumped );
  }

  // Remaining Data Types
  for ( std::vector<DataType>::const_iterator it = types.begin();
        it != types.end();
        ++it )
  {
    modFile += it->getModFileFullDefinition("  ",alreadyDumped);
  }

  // Domain Services
  for ( std::vector<DomainService>::const_iterator it = domainServices.begin();
        it != domainServices.end();
        ++it )
  {
    modFile += it->getModFileDeclaration("  ") + "\n";
  }

  // Bridges
  for ( std::vector<Bridge>::const_iterator it = bridges.begin();
        it != bridges.end();
        ++it )
  {
    modFile += it->getModFileDeclaration("  ") + "\n";
  }

  // Scenarios
  for ( std::vector<Scenario>::const_iterator it = scenarios.begin();
        it != scenarios.end();
        ++it )
  {
    modFile += it->getModFileDeclaration("  ") + "\n";
  }

  // Externals
  for ( std::vector<External>::const_iterator it = externals.begin();
        it != externals.end();
        ++it )
  {
    modFile += it->getModFileDeclaration("  ") + "\n";
  }



  if (!DumpOptions::instance().hasNoRelShips()){
      // Relationships
      const iUMLAPI::XUML::UMLAssociation_Set& relationships = analysis.getContainsUMLAssociationR225();

      for ( iUMLAPI::XUML::UMLAssociation_Set::const_iterator it = relationships.begin();
            it != relationships.end();
            ++it )
      {
        modFile += Relationship(*it).getModFileDeclaration("  ") + "\n";
      }

      // SuperSubtypes
      const iUMLAPI::XUML::Generalisation_Set& supersubs = analysis.getContainsClassR224().getIsSuperclassInGeneralisationR12();

      for ( iUMLAPI::XUML::Generalisation_Set::const_iterator it = supersubs.begin();
            it != supersubs.end();
            ++it )
      {
        modFile += SuperSubtype(*it).getModFileDeclaration("  ") + "\n";
      }
  }

  // Objects
  // Any supertypes must be dumped before their respective 
  // subtypes, so may need to do a number of passes until all 
  // objects have been dumped. There's probably a much more 
  // efficient way of doing this, but this way keeps the order 
  // the same as with the legacy dumper (as it's basically the 
  // same algorithm) 
  std::set<long> dumpedObjects;
  while ( dumpedObjects.size() != objects.size() )
  {
    for ( std::vector<Object>::const_iterator it = objects.begin();
          it != objects.end();
          ++it )
    {
      // Check that we have not already been dumped
      if ( !dumpedObjects.count(it->getNumber()) )
      {
        // Check that all supertypes have already been dumped, 
        // and delay dumping until the next pass if not. 
        bool dumpIt = true;
        for ( iUMLAPI::XUML::Class_Set::const_iterator sup = it->getSupertypes().begin();
              sup != it->getSupertypes().end();
              ++sup )
        {
          dumpIt &= dumpedObjects.count(sup->getClassNumber());
        }

        // OK to dump?
        if ( dumpIt )
        {
          modFile += it->getModFileDefinition("  ") + "\n";
          dumpedObjects.insert(it->getNumber());
        }
      }
    }
  }

  modFile += getSignOff();
  
  return modFile;
}


MaslGen::Domain::Interface::Interface( const Domain& domain )
  : domain(domain)
{ 
}

std::string MaslGen::Domain::Interface::getFileName() const
{
  return domain.getKeyLetter() + ".int";
}

std::string MaslGen::Domain::Interface::getFileContents() const
{
  std::string intFile = getCopyrightNotice() + wrapDescription("",domain.getDescription(),60) + domain.getDeclaration();

  // Public Data Types
  std::set<std::string> alreadyDumped;
  for ( std::vector<DataType>::const_iterator it = domain.types.begin();
        it != domain.types.end();
        ++it )
  {
    if ( it->isPublic() )
    {
      intFile += it->getModFileFullDefinition("  ",alreadyDumped);
    }
  }

  // Domain Service Parameter Data Types
  for ( std::vector<DomainService>::const_iterator it = domain.domainServices.begin();
        it != domain.domainServices.end();
        ++it )
  {
    if ( it->isPublic() )
    {
      intFile += it->getParameterTypeDefs ( "  ", alreadyDumped );
    }
  }

  // Terminator Service Parameter Data Types
  for ( std::vector<Bridge>::const_iterator it = domain.bridges.begin();
        it != domain.bridges.end();
        ++it )
  {
    intFile += it->getServiceParameterTypeDefs ( "  ", alreadyDumped );
  }

  // Domain Services
  for ( std::vector<DomainService>::const_iterator it = domain.domainServices.begin();
        it != domain.domainServices.end();
        ++it )
  {
    if ( it->isPublic() )
    {
      intFile += it->getModFileDeclaration("  ") + "\n";
    }
  }

  // Bridges
  for ( std::vector<Bridge>::const_iterator it = domain.bridges.begin();
        it != domain.bridges.end();
        ++it )
  {
    intFile += it->getModFileDeclaration("  ") + "\n";
  }



  intFile += domain.getSignOff();
  return intFile;
}

std::string MaslGen::Domain::Interface::getCopyrightPrefix() const
{
  return "//";
}

std::string MaslGen::Domain::Interface::getDescription() const
{
  return domain.getDescription();
}



