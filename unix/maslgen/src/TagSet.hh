// 
// Filename : TagSet.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_TagSet_HH
#define MaslGen_TagSet_HH

#include <map>
#include <vector>
#include <string>

#include "iumlapi/TAGS_TaggedItem.hh"

namespace MaslGen
{

  class TagSet
  {
    public:    
      TagSet( const iUMLAPI::TAGS::TaggedItem& taggedItem );

      bool hasTag ( const std::string& name ) const;
      const std::string& getTag ( const std::string& name ) const;

      const std::vector<std::pair<std::string,std::string> >& getTags() const;

    private:
      std::map<std::string,std::string> tags;
      std::vector<std::pair<std::string,std::string> > tagvec;
  };

}

#endif
