// 
// Filename : ShowVersions.cc
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
#include "iumlapi/ORG_User.hh"

#include <string>
#include "sys/stat.h"
#include <signal.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "boost/lexical_cast.hpp"

extern "C"
{
void quitGracefully ( int signal )
{
  sigignore(SIGINT);
  sigignore(SIGQUIT);
  throw std::string("Exiting");
}
}

void usage(const std::string& command)
{
  std::cerr << "Usage: " << command << " <database> [<keyLetter>|-tf|-pf|-u]" << std::endl; 
}

int main( int argc, char** argv)
{
  try
  {
    signal(SIGINT,quitGracefully);
    signal(SIGQUIT,quitGracefully);

    std::vector<std::string> options;
    char** opts = argv+1;
    while ( --argc ) options.push_back(*opts++);

    if ( options.size() < 1 || options.size() > 2 )
    {
      usage(argv[0]);
      return 2;
    }

    std::string dbname = options[0];
    std::string keyLetter = (options.size()>1 ? options[1] : "");

    iUMLAPI::Session session(dbname);
    iUMLAPI::ORG::Database database = session.getDatabase();

    if (keyLetter == "-pf" || keyLetter == "-tf" || keyLetter == "-u" || keyLetter == "" )
    {
      for ( iUMLAPI::ORG::Project_Set::const_iterator it = database.getContainsProjectR5().begin();
            it != database.getContainsProjectR5().end();
            ++it )
      {
        long version;
        if ( keyLetter == "-pf" )      version = it->getLatestFrozenVersionNo();
        else if ( keyLetter == "-tf" ) version = it->getLatestLockedVersionNo();
        else if ( keyLetter == "-u" )  version = it->getLatestUnlockedVersionNo();
        else                           version = it->getLatestVersionNo();


        if ( version > 0 )
        {
          const iUMLAPI::ORG::ProjectVersion& ver = it->getProjectVersion(version);
          std::string frozen;
          if ( keyLetter == "" ) frozen = (ver.getFrozen()?" (PF)":(ver.getLocked()?" (TF)":" (U)"));
          std::cout << it->getKeyLetter() << " " << version << " " << ver.getLatestBuildSetNo() << frozen << std::endl;
        }
      }

      for ( iUMLAPI::ORG::Domain_Set::const_iterator it = database.getContainsDomainR3().begin();
            it != database.getContainsDomainR3().end();
            ++it )
      {
        long version;
        if ( keyLetter == "-pf" )      version = it->getLatestFrozenVersionNo();
        else if ( keyLetter == "-tf" ) version = it->getLatestLockedVersionNo();
        else if ( keyLetter == "-u" )  version = it->getLatestUnlockedVersionNo();
        else                           version = it->getLatestVersionNo();

        if ( version > 0 )
        {
          const iUMLAPI::ORG::DomainVersion& ver = it->getDomainVersion(version);
          std::string frozen;
          if ( keyLetter == "" ) frozen = (ver.getFrozen()?" (PF)":(ver.getLocked()?" (TF)":" (U)"));
          std::cout << it->getKeyLetter() << " " << version  << frozen << std::endl;
        }
      }
    }
    else if ( database.projectExists(keyLetter) )
    {
      iUMLAPI::ORG::Project project = database.getProject(keyLetter);
      for ( iUMLAPI::ORG::ProjectVersion_Set::const_iterator it = project.getHasProjectVersionR6().begin();
            it != project.getHasProjectVersionR6().end();
            ++it )
      {
        std::cout << std::setw(2) << it->getVersionNumber() << " " 
                  << (it->getFrozen()?"(PF) ":(it->getLocked()?"(TF) ":"(U)   ")) 
                  << it->getName() << std::endl;

        for ( iUMLAPI::ORG::BuildSet_Set::const_iterator bs = it->getHasBuildSpecificationDefinedByBuildSetR20().begin();
              bs != it->getHasBuildSpecificationDefinedByBuildSetR20().end();
              ++bs )
        {
          std::cout << "              : " 
                    << std::setw(2) << bs->getBuildSetNumber() << " " 
                    << bs->getBuildSetName() << std::endl;
        }
     }
    }
    else
    {
      iUMLAPI::ORG::Domain domain = database.getDomain(keyLetter);
      for ( iUMLAPI::ORG::DomainVersion_Set::const_iterator it = domain.getHasDomainVersionR4().begin();
            it != domain.getHasDomainVersionR4().end();
            ++it )
      {
        std::cout << std::setw(2) << it->getVersionNumber() << " " 
                  << (it->getFrozen()?"(PF) ":(it->getLocked()?"(TF) ":"(U)   ")) 
                  << it->getName() << std::endl;
      }

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
