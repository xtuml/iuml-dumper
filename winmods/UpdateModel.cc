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

#include "Project.hh"
#include "Domain.hh"

#include <string>
#include "sys/stat.h"
#include <signal.h>
#include <vector>
//#include <sys/unistd.h>
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

using namespace MaslGen;

void usage(const std::string& command)
{
  std::cerr << "Usage: " << command << " <database> <keyLetter> <version> [<buildSet>] [<file> ... ]" << std::endl; 
}

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
    iUMLAPI::ORG::Database database = session.getDatabase();

    if ( database.projectExists(keyLetter) )
    {
      if ( options.size() < 4 ) 
      {
        usage(argv[0]);
        return 2;
      }

      std::set<std::string> filesToDump;
      std::copy(options.begin()+4,options.end(),std::inserter(filesToDump,filesToDump.end()));
      CodeFile::setFilesToDump(filesToDump);

      long buildSetNo;
      try
      {
        buildSetNo = boost::lexical_cast<long>(options[3]);
      }
      catch ( boost::bad_lexical_cast e ) 
      {
        usage(argv[0]);
        return 2;
      }

      iUMLAPI::ORG::Project umlProject = database.getProject(keyLetter);
      if ( umlProject.getLatestVersionNo() != versionNo )
      {
        std::cerr << "Warning: Updating version " << versionNo 
                  << " when later version " << umlProject.getLatestVersionNo() << " exists." << std::endl;
      }

      iUMLAPI::ORG::ProjectVersion umlProjectVersion = umlProject.getProjectVersion(versionNo);
      if ( umlProjectVersion.getLatestBuildSetNo() != buildSetNo )
      {
        std::cerr << "Warning: Updating build set " << buildSetNo 
                  << " when later version " << umlProjectVersion.getLatestBuildSetNo() << " exists." << std::endl;
      }

      umlProjectVersion.acquireLock();
      Project project( umlProjectVersion.getBuildSet(buildSetNo), database );
      project.updateFromFiles();
    }
    else
    {
      std::set<std::string> filesToDump;
      std::copy(options.begin()+3,options.end(),std::inserter(filesToDump,filesToDump.end()));
      CodeFile::setFilesToDump(filesToDump);

      iUMLAPI::ORG::Domain umlDomain = database.getDomain(keyLetter);
      if ( umlDomain.getLatestVersionNo() != versionNo )
      {
        std::cerr << "Warning: Dumping version " << versionNo 
                  << " when later version " << umlDomain.getLatestVersionNo() << " exists." << std::endl;
      }

      iUMLAPI::ORG::DomainVersion domainVersion = umlDomain.getDomainVersion(versionNo);

      domainVersion.acquireLock();

      Domain domain(domainVersion);
      domain.updateFromFiles();
    }

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
