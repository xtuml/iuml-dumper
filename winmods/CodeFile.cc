// 
// Filename : CodeFile.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "CodeFile.hh"
#include "TextUtils.hh"
#include <fstream>
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <Windows.h>

// This should just be a static member, but there appears to 
// be a bug in the sun linker/compiler, which crashes the 
// process when the member is accessed via a shared library. 
std::set<std::string>& filesToDump()
{
  static std::set<std::string> filesToDumpSet;   
  return filesToDumpSet;
}

MaslGen::CodeFile::CodeFile()
{
}

void MaslGen::CodeFile::dumpToFile() const
{
  if ( okToDump() )
  {
	CreateDirectory("output", NULL);

    std::cout << "Writing " << getFileName() << std::endl;
    std::ofstream file(("output\\" + getFileName()).c_str());
    file << getFileContents();
  }
}

void MaslGen::CodeFile::updateFromFile()
{
  if ( okToDump() )
  {
    std::ifstream file(getFileName().c_str());
    std::ostringstream newFileContents;
    newFileContents << file.rdbuf();

    if ( updateModel(newFileContents.str()) )
    {
      std::cout << "Updated:   " << getFileName() << std::endl;
    }
    else
    {
      std::cout << "No Change: " << getFileName() << std::endl;
    }
  }
}

bool MaslGen::CodeFile::updateModel(const std::string& fileContents)
{
  return false;
}


bool MaslGen::CodeFile::okToDump() const
{
  return hasCodeFile() && (!filesToDump().size() || filesToDump().count(getFileName()));
}

void MaslGen::CodeFile::setFilesToDump ( const std::set<std::string>& fileNames )
{
  filesToDump() = fileNames;
} 

std::string MaslGen::CodeFile::getCopyrightNotice() const
{
  int year = boost::gregorian::day_clock::local_day().year();

  return indentText(getCopyrightPrefix(),alignTabs(std::string("\n") +
                    " UK Crown Copyright (c) " + boost::lexical_cast<std::string>(year) + ". All rights reserved.\n" +
                    "\n")) + "\n";
}
