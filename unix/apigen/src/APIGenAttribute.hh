// 
// Filename : APIGenAttribute.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef APIGenAttribute_HH
#define APIGenAttribute_HH

#include <string>

namespace APIGen
{

  class Attribute
  {
    public:
      Attribute ( const std::string& line );

      bool isId() const { return id; }
      const std::string& getName() const { return name; }
      const std::string& getType() const { return type; }
      const std::string& getReference() const { return reference; }


      const std::string& getCppType() const           { return cppType; }     
      const std::string& getCppRefType() const        { return cppRefType; }
      const std::string& getCppTypeGetMethod() const  { return cppTypeGetMethod; }

      const std::string& getMemberName() const        { return memberName; }
      const std::string& getGetMethodName() const     { return getMethodName; }
      const std::string& getSetMethodName() const     { return setMethodName; }

    private:
      bool id;
      std::string name;
      std::string type;
      std::string reference;

      std::string cppType;
      std::string cppRefType;
      std::string cppTypeGetMethod;
      std::string memberName;
      std::string getMethodName;
      std::string setMethodName;

  };

  std::ostream& operator<< ( std::ostream& os, const Attribute& attribute );

};

#endif
