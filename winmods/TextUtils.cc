// 
// Filename : TextUtils.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "TextUtils.hh"
#include <algorithm>
#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>

char identChar ( char ch )
{
  switch(ch)
  {
    case ' '  :
    case '-'  :
    case '('  :
    case ')'  :
    case '\t' :
    case '\n' : return '_';
    default   : return ch;
  }
}

std::string MaslGen::makeIdentifier ( const std::string& value )
{
  std::string::size_type startPos = value.find_first_not_of(" \n\t");
  std::string::size_type endPos = value.find_last_not_of(" \n\t");
  if ( startPos == std::string::npos ) return "";

  std::string result;
  std::transform ( value.begin()+startPos,value.begin()+endPos+1,std::back_inserter(result),identChar);

  std::string::size_type pos = 0;
  while ( (pos = result.find("__",pos)) != std::string::npos )
  {
    result.erase(pos,1);
  }

  return result;
}

std::string MaslGen::alignTabs ( const std::string& value )
{
  std::vector<std::string::size_type> maxTabDistances;

  std::istringstream input(value);
  while ( input )
  {
    std::string line;
    std::getline ( input, line );

    std::string::size_type prevTabPos = 0;
    std::string::size_type tabPos = line.find('\t',prevTabPos);
    int tabStop = 0;
    while ( tabPos != std::string::npos )
    {
      if ( maxTabDistances.size() <= tabStop ) 
      {
        maxTabDistances.push_back(tabPos-prevTabPos);
      }
      else
      {
        maxTabDistances[tabStop] = std::max(tabPos-prevTabPos, maxTabDistances[tabStop]);
      }  
      ++tabStop;
      prevTabPos = tabPos+1;
      tabPos = line.find('\t',prevTabPos);
    }
  }

  std::string result;
  std::string::size_type lineStart = 0;
  std::string::size_type newlinePos = value.find('\n',lineStart);

  while ( lineStart != std::string::npos )
  {
    std::string line;
    if ( newlinePos == std::string::npos )
    {
      line = value.substr(lineStart);
    }
    else
    {
      line = value.substr(lineStart,newlinePos-lineStart+1);
    }
    std::string::size_type prevTabPos = 0;
    std::string::size_type tabPos = line.find('\t',prevTabPos);

    int tabStop = 0;
    while ( tabPos != std::string::npos )
    {
      std::string padding = std::string(maxTabDistances[tabStop]+prevTabPos-tabPos,' ');
      result.append(line,prevTabPos,tabPos-prevTabPos);
      result.append(padding);
      ++tabStop;
      prevTabPos = tabPos+1;
      tabPos = line.find_first_of('\t',prevTabPos);
    }
    result.append(line,prevTabPos,line.size()-prevTabPos);

    lineStart = newlinePos==std::string::npos ? std::string::npos : newlinePos+1;
    newlinePos = value.find('\n',lineStart);
  }
  return result;
}

std::string MaslGen::indentText ( const std::string& indent, const std::string& value )
{
  std::string result;
  std::string::size_type lineStart = 0;
  std::string::size_type newLinePos = 0;
  while ( (newLinePos = value.find('\n',lineStart)) != std::string::npos )
  {
    result.append(indent);
    result.append(value,lineStart,newLinePos-lineStart+1);
    lineStart = newLinePos+1;
  }
  if ( lineStart < value.size() )
  {
    // Special case for last line with no new line
    result.append(indent);
    result.append(value,lineStart,value.size()-lineStart);
  }
  return result;
}


std::string MaslGen::wrapText ( const std::string& value, int maxLineLen )
{
  std::string result;
  std::string::size_type lineStart = 0;
  std::string::size_type newLinePos = 0;
  while ( (newLinePos = value.find('\n',lineStart)) != std::string::npos )
  {
    result.append(wrapLine(std::string(value,lineStart,newLinePos-lineStart+1),maxLineLen));
    lineStart = newLinePos+1;
  }
  if ( lineStart < value.size() )
  {
    // Special case for last line with no new line
    result.append(wrapLine(std::string(value,lineStart,value.size()-lineStart),maxLineLen));
  }
  return result;  
}

std::string MaslGen::wrapLine ( const std::string& line, int maxLineLen )
{
  std::string result;

  std::string::size_type prefixLen = line.find_first_not_of(' ');
  std::string prefix = line.substr(0,prefixLen);
  std::string::size_type lineStart = prefixLen;
  while ( lineStart != std::string::npos )
  {
    std::string::size_type spacePos = line.rfind(' ',lineStart + maxLineLen - prefixLen);
    if ( spacePos <= lineStart || spacePos == std::string::npos )
    {
      // Can't wrap at less than maxLineLen, so find next opportunity 
      spacePos = line.find(' ',lineStart+maxLineLen - prefixLen);
    }
    if ( spacePos != std::string::npos && lineStart + maxLineLen < line.size() )
    {
      result.append(prefix);
      result.append(line,lineStart,spacePos-lineStart);
      result.append("\n");
      lineStart = spacePos + 1;
    }
    else
    {
      result.append(prefix);
      result.append(line,lineStart,line.size()-lineStart);
      lineStart = std::string::npos;
    }
  }
  return result;  
}


std::string MaslGen::wrapDescription ( const std::string& indent, const std::string& description, int maxLineLen )
{
  std::string result;
  if ( description.length() > 0 )
  {
    result = indentText(indent + "//! ",wrapText(description,maxLineLen));
    if ( result[result.length()-1] != '\n' )
    {
      result += '\n';
    }
  }

  return result;

}
