// 
// Filename : Callable.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Callable_HH
#define MaslGen_Callable_HH

#include "CodeFile.hh"

#include <string>

namespace MaslGen
{
  class Callable : public CodeFile
  {
    public:
      Callable();

      virtual std::string getFileName() const;
      virtual std::string getFileNamePrefix() const;
      virtual std::string getFileContents() const;
      virtual std::string getCopyrightPrefix() const;
      virtual bool updateModel(const std::string& fileContents);

      virtual std::string getDeclaration( bool fullyQualified ) const;
      virtual std::string getName( bool fullyQualified ) const;
      virtual std::string getNameQualification() const;
      virtual std::string getModFileDeclaration(const std::string& indent) const;
      virtual std::string getPragmas() const;

      virtual std::string getFileNameExtension() const = 0;
      virtual std::string getDomainName() const = 0;
      virtual std::string getCallableType() const = 0;
      virtual std::string getUnqualifiedName() const = 0;
      virtual std::string getMethod() const = 0;
      virtual std::string getParameterList() const = 0;
      virtual void updateMethod(const std::string& method) = 0;

  };
}

#endif
