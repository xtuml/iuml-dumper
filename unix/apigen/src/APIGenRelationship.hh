// 
// Filename : APIGenRelationship.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef APIGenRelationship_HH
#define APIGenRelationship_HH

#include <string>
#include <ostream>

namespace APIGen
{

  class Relationship
  {
    public:
      Relationship ( const std::string& line );
      Relationship ( const std::string& name, const std::string& supertype, const std::string& subtype, bool toSuper );
      const std::string& getName() const { return name; }
      const std::string& getFromObject() const { return fromObject; }
      bool isConditional() const { return conditional; }
      const std::string& getRole() const { return role; }
      bool isMany() const { return many; }
      const std::string& getToObject() const { return toObject; }
      bool isAssoc() const { return assoc; }
      bool isSuperSub() const { return superSub; }

    private:
      std::string name;
      std::string fromObject;
      bool conditional;
      std::string role;
      bool many;
      std::string toObject;
      bool assoc;
      bool superSub;
  };

  std::ostream& operator<< ( std::ostream& os, const Relationship& rel );

};
#endif
