// 
// Filename : FreezeVersion.cc
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
  std::cerr << "Usage: " << command << "{ -tf | -pf | -u } <database>  <keyLetter> <version>" << std::endl; 
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

    if ( options.size() != 4 )
    {
      usage(argv[0]);
      return 2;
    }

    std::string type = options[0];
    std::string dbname = options[1];
    std::string keyLetter = options[2];
    long versionNo;
    try
    {
      versionNo = boost::lexical_cast<long>(options[3]);
    }
    catch ( boost::bad_lexical_cast e ) 
    {
      usage(argv[0]);
      return 2;
    }

    iUMLAPI::FreezeType freezeType;
    std::string action;

    if ( type == "-tf" )
    {
      freezeType = iUMLAPI::temporary;
      action = "Temporarily Frozen";
    }
    else if ( type == "-pf" )
    {
      freezeType = iUMLAPI::permanent;
      action = "Permanently Frozen";
    }
    else if ( type == "-u" )
    {
      freezeType = iUMLAPI::unfreeze;
      action = "Unfrozen";
    }
    else
    {
      usage(argv[0]);
      return 2;
    }


    iUMLAPI::Session session(dbname,true);
    iUMLAPI::ORG::Database database = session.getDatabase();
    if ( freezeType == iUMLAPI::unfreeze ) database.acquireLock();

    if (database.projectExists(keyLetter) )
    {
      iUMLAPI::ORG::ProjectVersion ver = database.getProject(keyLetter).getProjectVersion(versionNo);
      if ( freezeType != iUMLAPI::unfreeze ) ver.acquireLock();
      ver.freeze(freezeType);
      std::cout << action << " Project " << keyLetter << " version " << versionNo << std::endl;
    }
    else
    {
      iUMLAPI::ORG::DomainVersion ver = database.getDomain(keyLetter).getDomainVersion(versionNo);
      if ( freezeType != iUMLAPI::unfreeze ) ver.acquireLock();
      ver.freeze(freezeType);
      std::cout << action << " Domain " << keyLetter << " version " << versionNo << std::endl;
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
