// 
// Filename : Callable.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Callable.hh"
#include "TextUtils.hh"
#include "Parameter.hh"

#include <iostream>

MaslGen::Callable::Callable()
{
}

std::string MaslGen::Callable::getName ( bool fullyQualified ) const
{
  return (fullyQualified?getNameQualification():"") + getUnqualifiedName();
}

std::string MaslGen::Callable::getFileName() const
{
  return getFileNamePrefix() + getName(false) + "." + getFileNameExtension();
}

std::string MaslGen::Callable::getNameQualification() const
{
  return getDomainName() + "::";
}


std::string MaslGen::Callable::getFileNamePrefix() const
{
  return "";
}

std::string MaslGen::Callable::getPragmas() const
{
  return "";
}

std::string MaslGen::Callable::getCopyrightPrefix() const
{
  return "//";
}

std::string MaslGen::Callable::getDeclaration ( bool fullyQualified ) const
{
  return getCallableType() + " " + getName(fullyQualified) + " " + getParameterList();
}

std::string MaslGen::Callable::getFileContents() const
{
  std::string method = getMethod();
  if ( method == "" )
  {
    method = "begin\n  null;\nend;\n";
  }
  return getCopyrightNotice() + wrapDescription("",getDescription(),1000) + alignTabs(getDeclaration(true)) + " is\n" + method; 
}

bool MaslGen::Callable::updateModel(const std::string& newFileContents)
{
  std::string header = alignTabs(getDeclaration(true)) + " is\n";

  // Skip anything before the header (eg copyright)
  std::string::size_type newHeaderPos = newFileContents.find(header);

  if ( newHeaderPos == std::string::npos )
  {
    std::cerr << "Warning: Declaration in " << getFileName() << " has changed. Cannot Update." << std::endl;
    std::cerr << "Expected : \n" << header << std::endl;
    return false;
  }
  else
  {
    std::string newMethod = newFileContents.substr(newHeaderPos+header.size());

    std::string curFileContents = getFileContents();
    std::string::size_type curHeaderPos = curFileContents.find(header);
    std::string curMethod = curFileContents.substr(curHeaderPos+header.size());
    
    if ( newMethod != curMethod )
    {
      if ( newMethod == "begin\n  null;\nend;\n")
      {
        newMethod = "";
      }

      updateMethod(newMethod);
      return true;
    }
    else
    {
      return false;
    }
  }
}

std::string MaslGen::Callable::getModFileDeclaration(const std::string& indent) const
{
  return wrapDescription(indent,getDescription(),1000) + 
         alignTabs(indent + getDeclaration(false)) + ";" + getPragmas() + "\n"; 
}
