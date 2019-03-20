// 
// Filename : External.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_External_HH
#define MaslGen_External_HH

#include <string>
#include "iumlapi/XUML_TestMethod.hh"
#include "TagSet.hh"
#include "Callable.hh"

namespace MaslGen
{
  class External : public Callable
  {
    public:
      External ( const iUMLAPI::XUML::TestMethod& test );

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
      iUMLAPI::XUML::TestMethod test;
      TagSet tags;

    private:
      static const std::string& nativeTag();
  };
}

#endif
