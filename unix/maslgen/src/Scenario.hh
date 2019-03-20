// 
// Filename : Scenario.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Scenario_HH
#define MaslGen_Scenario_HH

#include <string>
#include "iumlapi/XUML_InitialisationSegment.hh"
#include "TagSet.hh"
#include "Callable.hh"

namespace MaslGen
{
  class Scenario : public Callable
  {
    public:
      Scenario ( const iUMLAPI::XUML::InitialisationSegment& initSeg );

      virtual bool isNative() const;
      virtual bool hasCodeFile() const { return !isNative(); }
      virtual std::string getFileNameExtension() const;
      virtual std::string getDescription() const;
      virtual std::string getDomainName() const;
      virtual std::string getCallableType() const;
      virtual std::string getUnqualifiedName() const;
      virtual std::string getMethod() const;
      virtual std::string getParameterList() const;
      virtual std::string getPragmas() const;
      virtual const TagSet& getTagSet() const { return tags; }
      virtual void updateMethod(const std::string& method);

    private:
      iUMLAPI::XUML::InitialisationSegment initSeg;
      TagSet tags;

    private:
      static const std::string& nativeTag();
  };
}

#endif
