//
// UK Crown Copyright (c) 2016. All Rights Reserved.
//
#ifndef MaslTags_HH
#define MaslTags_HH


#include "iumlapi/TAGS_TagOwner.hh"
#include "iumlapi/TAGS_Tag.hh"
#include "iumlapi/XUML_Operation.hh"
#include "iumlapi/XUML_Attribute.hh"

class MaslTags
{
  public:
    MaslTags ( iUMLAPI::TAGS::TagOwner tagOwner );

    void convert( const iUMLAPI::XUML::Operation& domainService );
    void convert( const iUMLAPI::XUML::Attribute& attribute );

  private:
    void cleanup ( iUMLAPI::TAGS::Tag& tag );

    iUMLAPI::TAGS::Tag nativeTag; 
    iUMLAPI::TAGS::Tag uniqueTag; 
    iUMLAPI::TAGS::Tag functionTag; 
    iUMLAPI::TAGS::Tag startupTag; 
    iUMLAPI::TAGS::Tag signalHandlerTag; 
    iUMLAPI::TAGS::Tag indexTag; 
    iUMLAPI::TAGS::Tag testOnlyTag; 
    iUMLAPI::TAGS::Tag visibilityTag; 

    static const char* const GROUP_NAME;
    static const char* const NATIVE_TAG;
    static const char* const UNIQUE_TAG;
    static const char* const FUNCTION_TAG;
    static const char* const SIGNAL_HANDLER_TAG;
    static const char* const STARTUP_TAG;
    static const char* const INDEX_TAG;
    static const char* const NATIVE_SERVICE_TAG;
    static const char* const TEST_ONLY_TAG;
    static const char* const VISIBILITY_TAG;

};


#endif

