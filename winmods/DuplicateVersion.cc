// 
// Filename : DuplicateVersion.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/Session.hh"
#include "iumlapi/Utils.hh"
#include "iumlapi/ORG_Database.hh"
#include "iumlapi/ORG_Domain.hh"
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
  std::cerr << "Usage: " << command << "<database> <keyLetter> <version> <new name>" << std::endl; 
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
    std::string name = options[3];

    iUMLAPI::Session session(dbname,true);
    iUMLAPI::ORG::Database database = session.getDatabase();

    database.acquireLock();

    if (database.projectExists(keyLetter) )
    {
      iUMLAPI::ORG::Project project = database.getProject(keyLetter);
      project.duplicateVersion(versionNo,name);
      std::cout << "Created Project " << keyLetter << " version " << project.getLatestVersionNo() << std::endl;
    }
    else
    {
      iUMLAPI::ORG::Domain domain = database.getDomain(keyLetter);
      domain.duplicateVersion(versionNo,name);
      std::cout << "Created Domain " << keyLetter << " version " << domain.getLatestVersionNo() << std::endl;
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
