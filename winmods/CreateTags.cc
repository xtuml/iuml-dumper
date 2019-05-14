// 
// Filename : UpdateModel.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/Session.hh"
#include "iumlapi/Utils.hh"
#include "iumlapi/ORG_Database.hh"
#include "iumlapi/ORG_Domain.hh"
#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/ORG_Project.hh"
#include "iumlapi/ORG_ProjectVersion.hh"
#include "iumlapi/ORG_BuildSet.hh"
#include "iumlapi/ORG_StandardDomainVersion.hh"
#include "iumlapi/XUML_AnalysisArea.hh"
#include "iumlapi/XUML_DomainOperation.hh"


#include "MaslTags.hh"
#include "TranslatorTags.hh"

#include <string>
#include "sys/stat.h"
#include <signal.h>
#include <vector>
//#include <unistd.h>
#include <iostream>

#include "boost/lexical_cast.hpp"

extern "C"
{
void quitGracefully ( int signal )
{
  //sigignore(SIGINT);
  //sigignore(SIGQUIT);
  throw std::string("Exiting");
}
}

void usage(const std::string& command)
{
  std::cerr << "Usage: " << command << " <database> <keyLetter> <version> [<buildSet>]" << std::endl; 
}


using namespace iUMLAPI::ORG;

void createDomainTags ( const DomainVersion& domainVersion );
void createProjectTags ( const ProjectVersion& projectVersion );

int main( int argc, char** argv)
{
  try
  {
    signal(SIGINT,quitGracefully);
    //signal(SIGQUIT,quitGracefully);

    std::vector<std::string> options;
    char** opts = argv+1;
    while ( --argc ) options.push_back(*opts++);

    if ( options.size() < 3 ) 
    {
      usage(argv[0]);
      return 2;
    }

    std::string dbname = options[0];
    std::string keyLetter = options[1];

    long versionNo;
    try
    {
      versionNo = boost::lexical_cast<long>(options[2]);
    }
    catch ( boost::bad_lexical_cast e ) 
    {
      usage(argv[0]);
      return 2;
    }

    iUMLAPI::Session session(dbname,true);
    session.abortOnExit();
    Database database = session.getDatabase();
    if ( database.projectExists(keyLetter) )
    {
      Project umlProject = database.getProject(keyLetter);
      if ( umlProject.getLatestVersionNo() != versionNo )
      {
        std::cerr << "Warning: Updating version " << versionNo 
                  << " when later version " << umlProject.getLatestVersionNo() << " exists." << std::endl;
      }

      ProjectVersion umlProjectVersion = umlProject.getProjectVersion(versionNo);

      umlProjectVersion.acquireLock();
      createProjectTags( umlProjectVersion );
    }
    else
    {
      Domain umlDomain = database.getDomain(keyLetter);
      if ( umlDomain.getLatestVersionNo() != versionNo )
      {
        std::cerr << "Warning: Updating version " << versionNo 
                  << " when later version " << umlDomain.getLatestVersionNo() << " exists." << std::endl;
      }

      DomainVersion domainVersion = umlDomain.getDomainVersion(versionNo);

      domainVersion.acquireLock();

      createDomainTags(domainVersion);
    }
    
    // If we have got here, everything must be OK, so OK to commit
    session.commitOnExit();

  }
  catch ( const std::exception& error )
  { 
    std::cerr << "Exception: " << error.what() << std::endl;
    return 1;
  }
  catch ( ... )
  { 
    std::cerr << "Unrecognised Exception" << std::endl;
    return 1;
  }
  return 0;
}


void createDomainTags ( const DomainVersion& domainVersion )
{
  std::cout << "Creating domain tags on " << domainVersion.getIsAViewOfDomainR4().getKeyLetter() << " version " << domainVersion.getVersionNumber() << std::endl;

  MaslTags maslTags(domainVersion.getTAGS_TagOwner());
  TranslatorTags translatorTags(domainVersion.getTAGS_TagOwner());
}


void createProjectTags ( const ProjectVersion& projectVersion )
{
  std::cout << "Creating project tags on " << projectVersion.getIsAViewOfProjectR6().getKeyLetter() << " version " << projectVersion.getVersionNumber() << std::endl;
  TranslatorTags translatorTags(projectVersion.getTAGS_TagOwner());
}

