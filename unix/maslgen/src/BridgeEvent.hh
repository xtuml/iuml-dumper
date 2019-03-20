// 
// Filename : BridgeEvent.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_BridgeEvent_HH
#define MaslGen_BridgeEvent_HH

#include <string>
#include "iumlapi/XUML_TerminatorSignal.hh"
#include "EventAction.hh"
#include "TagSet.hh"

namespace MaslGen
{
  class BridgeEvent : public EventAction
  {
    public:
      BridgeEvent ( const iUMLAPI::XUML::TerminatorSignal& termSig );

      virtual std::string getCallableType() const;
      virtual std::string getNameQualification() const;
      virtual std::string getUnqualifiedName() const;
      virtual std::string getFileNamePrefix() const;
      virtual std::string getFileNameExtension() const;
      virtual std::string getMethod() const;
      virtual void updateMethod(const std::string& method);
      virtual const TagSet& getTagSet() const { return tags; }
      virtual std::string getPragmas() const;
      virtual bool isNative() const;
      virtual bool hasCodeFile() const { return !isNative(); }

      std::string getParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const;

    private:
      static const std::string& nativeTag();
      iUMLAPI::XUML::TerminatorSignal termSig;
      TagSet tags;
  };
}

#endif
