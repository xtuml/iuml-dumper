//
// UK Crown Copyright (c) 2016. All Rights Reserved.
//
#ifndef TranslatorTags_HH
#define TranslatorTags_HH


#include "iumlapi/TAGS_TagOwner.hh"
#include "iumlapi/TAGS_Tag.hh"
#include "iumlapi/XUML_Operation.hh"
#include "iumlapi/XUML_Attribute.hh"

class TranslatorTags
{
  public:
    TranslatorTags ( iUMLAPI::TAGS::TagOwner tagOwner );

  private:
    iUMLAPI::TAGS::Tag skipTag; 
    iUMLAPI::TAGS::Tag runTag; 
    iUMLAPI::TAGS::Tag onlyTag; 
 
    static const char* const GROUP_NAME;
    static const char* const SKIP_TAG;
    static const char* const RUN_TAG;
    static const char* const ONLY_TAG;

};


#endif

