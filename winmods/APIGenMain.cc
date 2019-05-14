// 
// Filename : APIGenMain.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenDomain.hh"
#include "APIGenObject.hh"
#include "APIGenAttribute.hh"
#include "APIGenRelationship.hh"

#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <Windows.h>

using namespace APIGen;

int main( int argc, char** argv)
{
  std::string outputdir = *++argv;
  --argc;

  std::cout << "Writing files to " << outputdir << std::endl;
  CreateDirectory(outputdir.c_str(), NULL);
  CreateDirectory((outputdir + "\\src").c_str(), NULL);
  CreateDirectory((outputdir + "\\include").c_str(), NULL);
  CreateDirectory((outputdir + "\\include\\iumlapi").c_str(), NULL);
  /*mkdir(outputdir.c_str(),0777);
  mkdir((outputdir + "/src").c_str(),0777);
  mkdir((outputdir + "/include").c_str(),0777);
  mkdir((outputdir + "/include/iumlapi").c_str(),0777);*/
  
  std::vector<std::string> options;
  while ( --argc ) options.push_back(*++argv);

  
  std::map<std::string,Domain> domains;

  for ( std::vector<std::string>::const_iterator optionIt = options.begin();
        optionIt != options.end(); ++optionIt )
  {
    try
    {
      Domains::addDomain(Domain(*optionIt));
    }
    catch ( const std::exception& e )
    {
      std::cout << "Error adding "<< *optionIt << ": " << e.what() << std::endl;
    }
  }

  std::ifstream counterparts("counterparts.txt");

  if ( !counterparts ) std::cout << "Warning: Unable to open counterparts.txt" << std::endl;

  while ( counterparts )
  {
    std::string fromDomain;
    std::string toDomain;
    std::string fromObject;
    std::string toObject;

    counterparts >> fromDomain >> fromObject >> toDomain >> toObject;
    if ( counterparts )
    {
      try
      {
        Domains::getDomain(fromDomain).getObject(fromObject).addCounterpart(toDomain,toObject);
      }
      catch ( const std::range_error& e )
      {
        std::cout << "Warning: counterparts.txt: " << e.what() << std::endl;
      }
    }
  }

  std::ifstream lookups("lookups.txt");

  if ( !lookups ) std::cout << "Warning: Unable to open lookups.txt" << std::endl;

  while ( lookups )
  {
    std::string fromDomain;
    std::string fromObject;
    std::string rel;
    std::string att;

    lookups >> fromDomain >> fromObject >> rel >> att;
    if ( lookups )
    {
      try
      {
        Domains::getDomain(fromDomain).getObject(fromObject).addLookup(rel,att);
      }
      catch ( const std::range_error& e )
      {
        std::cout << "Warning: lookups.txt: " << e.what() << std::endl;
      }
    }
  }

  std::ifstream creates("creates.txt");

  if ( !creates ) std::cout << "Warning: Unable to open creates.txt" << std::endl;

  std::string createLine;
  while ( std::getline(creates,createLine) )
  {
    std::istringstream line(createLine);

    std::string fromDomain;
    std::string fromObject;
    Object::ApiCreateFunction fnDef;

    line >> fromDomain >> fromObject >> fnDef.obj >> fnDef.function;
    if ( line )
    {
      while ( line )
      {
        std::string param;
        line >> param;
        if ( line ) fnDef.params.push_back(param);
      }

      try
      {
        Domains::getDomain(fromDomain).getObject(fromObject).addApiCreateFn(fnDef);
      }
      catch ( const std::range_error& e )
      {
        std::cout << "Warning: creates.txt: " << e.what() << std::endl;
      }
    }
  }



  std::ifstream deletes("deletes.txt");

  if ( !deletes ) std::cout << "Warning: Unable to open lookups.txt" << std::endl;

  while ( deletes )
  {
    std::string fromDomain;
    std::string fromObject;
    std::string fnName;

    deletes >> fromDomain >> fromObject >> fnName;
    if ( deletes )
    {
      try
      {
        Domains::getDomain(fromDomain).getObject(fromObject).setApiDeleteFn(fnName);
      }
      catch ( const std::range_error& e )
      {
        std::cout << "Warning: deletes.txt: " << e.what() << std::endl;
      }
    }
  }

  for (std::map<std::string,Domain>::iterator it = Domains::getDomains().begin(); it != Domains::getDomains().end(); ++it )
  {
    try
    {
      it->second.generateAPI(outputdir);
    }
    catch ( const std::exception& e )
    {
      std::cout << "Error generating "<< it->second.getName() << ": " << e.what() << std::endl;
    }
  }
}
