// 
// Filename : Service.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Service_HH
#define MaslGen_Service_HH

#include <string>
#include "iumlapi/XUML_Operation.hh"
#include "TagSet.hh"
#include "Callable.hh"
#include "Parameter.hh"

namespace MaslGen
{
  class Service : public Callable
  {
    public:
      Service ( const iUMLAPI::XUML::Operation& operation );

      virtual ~Service() {}

      virtual std::string getFileNameExtension() const;
      virtual std::string getDescription() const;

      virtual bool isPublic() const;
      virtual bool isNative() const;
      virtual bool hasCodeFile() const { return !isNative(); }
      virtual bool isFunction() const;
      virtual std::string getCallableType() const;
      virtual std::string getUnqualifiedName() const;
      virtual std::string getDomainName() const;
      virtual std::string getMethod() const;
      virtual std::string getParameterList() const;
      virtual std::string getPragmas() const;
      virtual const TagSet& getTagSet() const { return tags; }
      virtual void updateMethod(const std::string& method);

      std::string getParameterTypeDefs ( const std::string& indent, std::set<std::string>& alreadyDefined ) const;

    protected:
      iUMLAPI::XUML::Operation operation;
      TagSet tags;

    private:
      static const std::string& nativeTag();
      static const std::string& functionTag();
      static const std::string& beginNative();
      static const std::string& endNative();
      static const std::string& visibilityTag();

      mutable std::vector<Parameter> inParamters;
      mutable std::vector<Parameter> outParamters;

  };
}

#endif
