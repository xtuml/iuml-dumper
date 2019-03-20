// 
// Filename : SupplementaryCodeBlock.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "SupplementaryCodeBlock.hh"
#include "TextUtils.hh"

std::string convertFileName ( std::string filename )
{
  for ( std::string::iterator it = filename.begin(); it != filename.end(); ++it )
  {
    if ( *it == ' ' ) *it = '_';
  }
  return filename;
}

MaslGen::SupplementaryCodeBlock::SupplementaryCodeBlock ( const iUMLAPI::XUML::SupplementaryCodeBlock& scb )
  : domainScb(scb),
    filename(convertFileName(scb.getName()))
{
}

MaslGen::SupplementaryCodeBlock::SupplementaryCodeBlock ( const iUMLAPI::BS::SupplementaryCodeBlock& scb )
  : projectScb(scb),
    filename(convertFileName(scb.getName()))
{
}


std::string MaslGen::SupplementaryCodeBlock::getFileContents() const
{
  return (domainScb.isValid()?domainScb.getAsl():projectScb.getAsl()) + "\n";
}

bool MaslGen::SupplementaryCodeBlock::updateModel(const std::string& fileContents)
{
  if ( fileContents != getFileContents() )
  {
    domainScb.isValid()?
      domainScb.setAsl(fileContents.substr(0,fileContents.size()-1)) :
      projectScb.setAsl(fileContents.substr(0,fileContents.size()-1));
    return true;
  }
  else
  {
    return false;
  }
}
