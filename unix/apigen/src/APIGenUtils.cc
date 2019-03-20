// 
// Filename : APIGenUtils.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "APIGenUtils.hh"
#include <cctype>

using std::toupper;

std::string APIGen::camelCase ( const std::string& value, bool capitaliseFirst, bool lowerFirst )
{
  bool capitaliseNext = capitaliseFirst;
  bool lowerNext = lowerFirst;
  std::string result;
  for ( std::string::const_iterator it = value.begin();
        it != value.end();
        ++it )
  {
    if ( *it == '_' )
    {
      capitaliseNext = true;
    }
    else if ( lowerNext )
    {
      result.push_back(tolower(*it));
      lowerNext = false;
    }
    else if ( capitaliseNext )
    {
      result.push_back(toupper(*it));
      capitaliseNext = false;
    }
    else
    {
      result.push_back(*it);
    }
  }
  return result;
}

