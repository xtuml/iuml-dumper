// 
// Filename : Project.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Project.hh"
#include "TextUtils.hh"

#include "iumlapi/BS_DomainVersionInBuildSet.hh"
#include "iumlapi/BS_TerminatorSignalBridge.hh"
#include "iumlapi/BS_TerminatorOperationBridge.hh"
#include "iumlapi/BS_NonCounterpartTerminator.hh"
#include "iumlapi/BS_SupplementaryCodeBlock.hh"

#include "iumlapi/ORG_Database.hh"
#include "iumlapi/ORG_Domain.hh"
#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/ORG_StandardDomainVersion.hh"
#include "iumlapi/ORG_BuildSet.hh"
#include "iumlapi/ORG_Project.hh"
#include "iumlapi/ORG_ProjectVersion.hh"

#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_Operation.hh"
#include "iumlapi/XUML_NonCounterpartTerminatorOperation.hh"
#include "iumlapi/XUML_NonCounterpartTerminator.hh"
#include "iumlapi/XUML_SignalProducerConsumer.hh"
#include "iumlapi/XUML_UMLSignal.hh"
#include "iumlapi/XUML_TerminatorSignal.hh"

#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include <map>

MaslGen::Project::Project ( const iUMLAPI::ORG::BuildSet& buildSet, const iUMLAPI::ORG::Database& database )
  : buildSet(buildSet.getBS_BuildSet()),
    database(database),
    cacheInitialised(false)
{
  

}

void MaslGen::Project::updateFromFiles()
{
  initCache();

  // Dump the .prj file
  updateFromFile();
  std::for_each ( scbs.begin(), scbs.end(), std::mem_fun_ref(&CodeFile::updateFromFile) );
  std::for_each ( domains.begin(), domains.end(), std::mem_fun_ref(&ProjectDomain::updateFromFiles) );
}

void MaslGen::Project::dumpToFiles() const
{
  initCache();

  // Dump the .prj file
  dumpToFile();
  std::for_each ( scbs.begin(), scbs.end(), std::mem_fun_ref(&CodeFile::dumpToFile) );
  std::for_each ( domains.begin(), domains.end(), std::mem_fun_ref(&ProjectDomain::dumpToFiles) );

}

void MaslGen::Project::initCache() const
{
  if ( !cacheInitialised )
  {
    cacheInitialised = true;

	/*
    std::copy(buildSet.getContainsSupplementaryCodeBlockR999().begin(),
              buildSet.getContainsSupplementaryCodeBlockR999().end(),
              back_inserter(scbs));
	*/

    // Add all domains in the build set. 
    for ( iUMLAPI::BS::DomainVersionInBuildSet_Set::const_iterator dv = buildSet.getIncludesDomainVersionInBuildSetR112().begin();
          dv != buildSet.getIncludesDomainVersionInBuildSetR112().end();
          ++dv )
    {
      domains.push_back ( ProjectDomain (*dv, database) );
    }

  }
}

std::string MaslGen::Project::getFileName() const
{
  return buildSet.getProjectKeyLetter() + ".prj";
}

std::string MaslGen::Project::getCopyrightPrefix() const
{
  return "//";
}

std::string MaslGen::Project::getDescription() const
{
  return buildSet.getORG_BuildSet().getDefinesTheBuildForProjectVersionR20().getIsAViewOfProjectR6().getMissionStatement();
}



std::string MaslGen::Project::getFileContents() const
{
  std::string prjFile = wrapDescription("",getDescription(),60) + 
                "project " + buildSet.getProjectKeyLetter() + " is\n";

  for ( std::vector<ProjectDomain>::const_iterator it = domains.begin(); it != domains.end(); ++it )
  {
    prjFile+= it->getPrjFileDeclaration("  ");
  }

  prjFile+= "end project;\n";
  TagSet tags(buildSet.getTAGS_TaggedItem());
  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = tags.getTags().begin();
        it != tags.getTags().end();
        ++it )
  {
    prjFile+= "pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");\n";
  }

  return getCopyrightNotice() + prjFile;
}
