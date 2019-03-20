//
// UK Crown Copyright (c) 2016. All Rights Reserved.
//
#include "TranslatorTags.hh"

const char* const TranslatorTags::GROUP_NAME = "translator";
const char* const TranslatorTags::RUN_TAG = "RunTranslator";
const char* const TranslatorTags::SKIP_TAG = "SkipTranslator";
const char* const TranslatorTags::ONLY_TAG = "OnlyTranslator";

#include "iumlapi/TAGS_TagGroup.hh"
#include "iumlapi/TAGS_TagAttachment.hh"
#include "iumlapi/TAGS_TaggedItem.hh"

TranslatorTags::TranslatorTags ( iUMLAPI::TAGS::TagOwner tagOwner )
{
  iUMLAPI::TAGS::TagGroup translatorGroup;

  for ( iUMLAPI::TAGS::TagGroup_Set::const_iterator tagGroupIt = tagOwner.getOwnsTagGroupR4().begin(), tagGroupsEnd = tagOwner.getOwnsTagGroupR4().end(); 
        tagGroupIt != tagGroupsEnd && !translatorGroup.isValid(); ++tagGroupIt )
  {
    if ( tagGroupIt->getName() == GROUP_NAME )
    {
      translatorGroup = *tagGroupIt;
    }
  }

  if ( !translatorGroup.isValid() )
  {
    std::cout << "Creating Tag Group " << GROUP_NAME << std::endl;
    translatorGroup = tagOwner.createTagGroup ( GROUP_NAME );
    translatorGroup.setDescription("Tags for configuring SOA");
  }

  for ( iUMLAPI::TAGS::Tag_Set::const_iterator it = translatorGroup.getContainsTagR3().begin(), end = translatorGroup.getContainsTagR3().end(); 
        it != end; ++it )
  {
    if ( it->getName() == RUN_TAG )
    {
      runTag = *it;
    }
    else if ( it->getName() == SKIP_TAG )
    {
      skipTag = *it;
    }
    else if ( it->getName() == ONLY_TAG )
    {
      onlyTag = *it;
    }
  }

  if ( !runTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << RUN_TAG << std::endl;
    runTag = translatorGroup.createTag ( RUN_TAG, iUMLAPI::singleValue );
    runTag.setDescription("Indicates an extra translator to be run on the domain or build set");
  }

  if ( !skipTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << SKIP_TAG << std::endl;
    skipTag = translatorGroup.createTag ( SKIP_TAG, iUMLAPI::singleValue );
    skipTag.setDescription("Indicates a translator to be skipped for the domain or build set");
  }

  if ( !onlyTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << ONLY_TAG << std::endl;
    onlyTag = translatorGroup.createTag ( ONLY_TAG, iUMLAPI::singleValue );
    onlyTag.setDescription("Indicates the only translator to be run on the domain or build set");
  }

}


