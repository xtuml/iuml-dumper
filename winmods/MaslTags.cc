//
// UK Crown Copyright (c) 2016. All Rights Reserved.
//
#include "MaslTags.hh"

const char* const MaslTags::GROUP_NAME = "masl";
const char* const MaslTags::NATIVE_TAG = "native";
const char* const MaslTags::UNIQUE_TAG = "unique";
const char* const MaslTags::VISIBILITY_TAG = "visibility";
const char* const MaslTags::FUNCTION_TAG = "function";
const char* const MaslTags::SIGNAL_HANDLER_TAG = "signal_handler";
const char* const MaslTags::STARTUP_TAG = "startup";
const char* const MaslTags::INDEX_TAG = "index";

const char* const MaslTags::NATIVE_SERVICE_TAG = "native_service";
const char* const MaslTags::TEST_ONLY_TAG = "test_only";

#include "iumlapi/TAGS_TagGroup.hh"
#include "iumlapi/TAGS_TagAttachment.hh"
#include "iumlapi/TAGS_TaggedItem.hh"
#include "iumlapi/TAGS_TagEnumeration.hh"
#include "iumlapi/XUML_Class.hh"

#include <iostream>

MaslTags::MaslTags ( iUMLAPI::TAGS::TagOwner tagOwner )
{
  iUMLAPI::TAGS::TagGroup maslGroup;

  for ( iUMLAPI::TAGS::TagGroup_Set::const_iterator tagGroupIt = tagOwner.getOwnsTagGroupR4().begin(), tagGroupsEnd = tagOwner.getOwnsTagGroupR4().end(); 
        tagGroupIt != tagGroupsEnd && !maslGroup.isValid(); ++tagGroupIt )
  {
    if ( tagGroupIt->getName() == GROUP_NAME )
    {
      maslGroup = *tagGroupIt;
    }
  }

  if ( !maslGroup.isValid() )
  {
     std::cout << "Creating Tag Group " << GROUP_NAME << std::endl;
    maslGroup = tagOwner.createTagGroup ( GROUP_NAME );
    maslGroup.setDescription("Tags for configuring SOA");
  }

  for ( iUMLAPI::TAGS::Tag_Set::const_iterator it = maslGroup.getContainsTagR3().begin(), end = maslGroup.getContainsTagR3().end(); 
        it != end; ++it )
  {
    if ( it->getName() == NATIVE_TAG )
    {
      nativeTag = *it;
    }
    else if ( it->getName() == UNIQUE_TAG )
    {
      uniqueTag = *it;
    }
    else if ( it->getName() == FUNCTION_TAG )
    {
      functionTag = *it;
    }
    else if ( it->getName() == SIGNAL_HANDLER_TAG )
    {
      signalHandlerTag = *it;
    }
    else if ( it->getName() == STARTUP_TAG )
    {
      startupTag = *it;
    }
    else if ( it->getName() == INDEX_TAG )
    {
      indexTag = *it;
    }
    else if ( it->getName() == TEST_ONLY_TAG )
    {
      testOnlyTag = *it;
    }
    else if ( it->getName() == VISIBILITY_TAG )
    {
      visibilityTag = *it;
    }
  }

  if ( !testOnlyTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << TEST_ONLY_TAG << std::endl;
    testOnlyTag = maslGroup.createTag ( TEST_ONLY_TAG, iUMLAPI::enumeration );
    testOnlyTag.addTagEnumeration("true");
    testOnlyTag.setDescription("Indicates that the domain service, scenario or external is only used during testing");
  }

  if ( !nativeTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << NATIVE_TAG << std::endl;
    nativeTag = maslGroup.createTag ( NATIVE_TAG, iUMLAPI::enumeration );
    nativeTag.addTagEnumeration("true");
    nativeTag.setDescription("Indicates that the service will be defined externally to the model compiler");
  }

  if ( !uniqueTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << UNIQUE_TAG << std::endl;
    uniqueTag = maslGroup.createTag ( UNIQUE_TAG, iUMLAPI::enumeration );
    uniqueTag.addTagEnumeration("true");
    uniqueTag.setDescription("Indicates that the attribute will be assigned a unique value by the architecture");
  }

  if ( !functionTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << FUNCTION_TAG << std::endl;
    functionTag = maslGroup.createTag ( FUNCTION_TAG, iUMLAPI::enumeration );
    functionTag.addTagEnumeration("true");
    functionTag.setDescription("Determines that the service has function semantics. The first out parameter will be used as a return value.");
  }

  if ( !signalHandlerTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << SIGNAL_HANDLER_TAG << std::endl;
    signalHandlerTag = maslGroup.createTag ( SIGNAL_HANDLER_TAG, iUMLAPI::singleValue );
    signalHandlerTag.setDescription("Determines that the service should be registered as a signal handler for the specified signal");
  }

  if ( !startupTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << STARTUP_TAG << std::endl;
    startupTag = maslGroup.createTag ( STARTUP_TAG, iUMLAPI::enumeration );
    startupTag.addTagEnumeration("true");
    startupTag.setDescription("Determines that the service should be run during initialisation of the process");
  }

  if ( !indexTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << INDEX_TAG << std::endl;
    indexTag = maslGroup.createTag ( INDEX_TAG, iUMLAPI::enumeration );
    indexTag.addTagEnumeration("true");
    indexTag.setDescription("A hint that the attribute may be a good candidate for indexing");
  }

  if ( !visibilityTag.isValid() )
  {
    std::cout << "Creating Tag (" << GROUP_NAME << ")" << VISIBILITY_TAG << std::endl;
    visibilityTag = maslGroup.createTag ( VISIBILITY_TAG, iUMLAPI::enumeration );
    visibilityTag.addTagEnumeration("public");
    visibilityTag.addTagEnumeration("private");
    visibilityTag.setDescription("Mark a type or service as public or private");
  }


}

void MaslTags::convert ( const iUMLAPI::XUML::Operation& operation )
{
  iUMLAPI::TAGS::TaggedItem taggedItem = operation.getTAGS_TaggedItem();
  iUMLAPI::TAGS::TagAttachment_Set attachments = taggedItem.getIsAttachedWithTagAttachmentR2();

  std::set<std::string> alreadyAttached;
  for ( iUMLAPI::TAGS::TagAttachment_Set::const_iterator it = attachments.begin(), end = attachments.end(); it != end; ++it )
  {
    iUMLAPI::TAGS::Tag tag = it->getIsAttachedWithTagR2();
    std::string tagName = tag.getName();
    std::transform(tagName.begin(),tagName.end(),tagName.begin(),tolower);
    alreadyAttached.insert(tagName);
  }

  for ( iUMLAPI::TAGS::TagAttachment_Set::const_iterator it = attachments.begin(), end = attachments.end(); it != end; ++it )
  {
    iUMLAPI::TAGS::Tag tag = it->getIsAttachedWithTagR2();
    std::string tagName = tag.getName();
    std::transform(tagName.begin(),tagName.end(),tagName.begin(),tolower);

    if ( tagName == NATIVE_TAG || tagName ==  NATIVE_SERVICE_TAG )
    {
      if ( !(tag == nativeTag) )
      {
        std::string tagValue = it->getTagValue();
        std::cout << operation.getName() << " : Removing Tag " 
                  << tag.getName() << "(" << tagValue <<  ")" << std::endl;
        taggedItem.detachTag(tag);

        std::cout << operation.getName() << " : Attaching Tag " 
                  << NATIVE_TAG << "(true)" << std::endl;
        taggedItem.attachTag ( nativeTag, "true" );

        cleanup(tag);
      }
    }
    else if ( tagName == FUNCTION_TAG )
    {
      if ( !(tag == functionTag) )
      {
        std::string tagValue = it->getTagValue();
        std::cout << operation.getName() << " : Removing Tag " 
                  << tag.getName() << "(" << tagValue <<  ")" << std::endl;
        taggedItem.detachTag(tag);

        std::cout << operation.getName() << " : Attaching Tag " 
                  << FUNCTION_TAG << "(true)" << std::endl;
        taggedItem.attachTag ( functionTag, "true" );

        cleanup(tag);
      }
    }
    else if ( tagName == SIGNAL_HANDLER_TAG )
    {
      if ( !(tag == signalHandlerTag) )
      {
        std::string tagValue = it->getTagValue();
        std::cout << operation.getName() << " : Removing Tag " 
                  << tag.getName()  << "(" << tagValue <<  ")" << std::endl;
        taggedItem.detachTag(tag);

        std::cout << operation.getName() << " : Attaching Tag " 
                  << SIGNAL_HANDLER_TAG << "(" + tagValue + ")" << std::endl;
        taggedItem.attachTag ( signalHandlerTag, tagValue );

        cleanup(tag);
      }
    }
    else if ( tagName == STARTUP_TAG )
    {
      if ( !(tag == startupTag) )
      {
        std::string tagValue = it->getTagValue();
        std::cout << operation.getName() << " : Removing Tag " 
                  << tag.getName()  << "(" << tagValue <<  ")" << std::endl;
        taggedItem.detachTag(tag);

        std::cout << operation.getName() << " : Attaching Tag " 
                  << STARTUP_TAG << "(" + tagValue + ")" << std::endl;
        taggedItem.attachTag ( startupTag, tagValue );

        cleanup(tag);
      }
    }
  }
}

void MaslTags::convert ( const iUMLAPI::XUML::Attribute& attribute )
{

  std::string attName = attribute.getCharacterisesClassR9().getClassName() + "." + attribute.getAttributeName();

  iUMLAPI::TAGS::TaggedItem taggedItem = attribute.getTAGS_TaggedItem();
  iUMLAPI::TAGS::TagAttachment_Set attachments = taggedItem.getIsAttachedWithTagAttachmentR2();

  std::set<std::string> alreadyAttached;
  for ( iUMLAPI::TAGS::TagAttachment_Set::const_iterator it = attachments.begin(), end = attachments.end(); it != end; ++it )
  {
    iUMLAPI::TAGS::Tag tag = it->getIsAttachedWithTagR2();
    std::string tagName = tag.getName();
    std::transform(tagName.begin(),tagName.end(),tagName.begin(),tolower);
    alreadyAttached.insert(tagName);
  }

  for ( iUMLAPI::TAGS::TagAttachment_Set::const_iterator it = attachments.begin(), end = attachments.end(); it != end; ++it )
  {
    iUMLAPI::TAGS::Tag tag = it->getIsAttachedWithTagR2();
    std::string tagName = tag.getName();
    std::transform(tagName.begin(),tagName.end(),tagName.begin(),tolower);

    if ( tagName == UNIQUE_TAG )
    {
      if ( !(tag == uniqueTag) )
      {
        std::string tagValue = it->getTagValue();
        std::cout << attName << " : Removing Tag " 
                  << tag.getName() << "(" << tagValue <<  ")" << std::endl;
        taggedItem.detachTag(tag);

        std::cout << attName << " : Attaching Tag " 
                  << UNIQUE_TAG << "(true)" << std::endl;
        taggedItem.attachTag ( uniqueTag, "true" );

        cleanup(tag);
      }
    }
  }
}

void MaslTags::cleanup ( iUMLAPI::TAGS::Tag& tag )
{
  if ( tag.getIsAttachedToTaggedItemR2().size() == 0 )
  {
    iUMLAPI::TAGS::TagGroup group = tag.getIsDefinedForTagGroupR3();
    std::cout << "Deleting Tag " << tag.getName() << std::endl;
    tag.remove();
    if ( group.getContainsTagR3().size() == 0 )
    {
      std::cout << "Deleting Tag Group " << group.getName() << std::endl;
      group.remove();
    }
  }
}



