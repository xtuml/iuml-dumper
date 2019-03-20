// 
// Filename : TagSet.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "TagSet.hh"
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

#include "iumlapi/TAGS_TagAttachment.hh"
#include "iumlapi/TAGS_TagEnumeration.hh"
#include "iumlapi/TAGS_Tag.hh"

char toLower(char c) { return std::tolower(c); }

MaslGen::TagSet::TagSet( const iUMLAPI::TAGS::TaggedItem& taggedItem )
{
  for ( iUMLAPI::TAGS::TagAttachment_Set::const_iterator it = taggedItem.getIsAttachedWithTagAttachmentR2().begin();
        it != taggedItem.getIsAttachedWithTagAttachmentR2().end();
        ++it )
  {
    std::string name = it->getIsAttachedWithTagR2().getName();
    std::transform(name.begin(),name.end(),name.begin(),toLower);

    iUMLAPI::TAGS::TagEnumeration enumerate = it->getHasEnumerationDefinedByTagEnumerationR7();
    if ( !enumerate.isValid() )
    {
      tags.insert(std::pair<std::string,std::string>(name,it->getTagValue()));
      tagvec.push_back(std::pair<std::string,std::string>(name,it->getTagValue()));
    }
    else
    {
      tags.insert(std::pair<std::string,std::string>(name,enumerate.getName()));
      tagvec.push_back(std::pair<std::string,std::string>(name,enumerate.getName()));
    }  
  }
}

bool MaslGen::TagSet::hasTag ( const std::string& name ) const
{
  return tags.count(name);
}

const std::string& MaslGen::TagSet::getTag ( const std::string& name ) const
{
  return tags.find(name)->second;
}

const std::vector<std::pair<std::string,std::string> >& MaslGen::TagSet::getTags() const
{
  return tagvec;
}
