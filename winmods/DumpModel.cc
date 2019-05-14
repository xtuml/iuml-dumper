// 
// Filename : DumpModel.cc
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
#include "DumpOptions.hh"

#include <string>
#include "sys/stat.h"
#include <signal.h>
#include <vector>
//#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "boost/lexical_cast.hpp"

#include <Windows.h>

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
  std::cerr << "Usage: " << command << DumpOptions::instance().getUsage() << " <database> <keyLetter> <version> [<buildSet>] [<file> ... ]" << std::endl; 
}

int main( int argc, char** argv)
{
  // Force a read to pause program, allowing for enough time for Tools > Attach to Process and thus for debugging the dumper
  std::cout << "Type any integer and hit enter after you have attached this to VS debugger. Don't forget to set breakpoint in the code";
  int age;
  std::cin >> age;
  try
  {    
    signal(SIGINT,quitGracefully);
    //signal(SIGQUIT,quitGracefully);

    char** opts = argv+1;
    std::vector<std::string> allOptions;
    while ( --argc ) allOptions.push_back(*opts++);    

    std::vector<std::string> options = DumpOptions::instance().stripOptions(allOptions);

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

    bool objectsOnly = false;
    if(std::find(options.begin(),options.end(),std::string("-obj-only")) != options.end()){
       objectsOnly = true;
    }

    iUMLAPI::Session session(dbname);
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
        std::cerr << "Warning: Dumping version " << versionNo 
                  << " when later version " << umlProject.getLatestVersionNo() << " exists." << std::endl;
      }

      iUMLAPI::ORG::ProjectVersion umlProjectVersion = umlProject.getProjectVersion(versionNo);
      if ( umlProjectVersion.getLatestBuildSetNo() != buildSetNo )
      {
        std::cerr << "Warning: Dumping build set " << buildSetNo 
                  << " when later version " << umlProjectVersion.getLatestBuildSetNo() << " exists." << std::endl;
      }

      Project project( umlProjectVersion.getBuildSet(buildSetNo), database );
      project.dumpToFiles();
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

      Domain domain(umlDomain.getDomainVersion(versionNo));
      domain.dumpToFiles();
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
