// 
// Filename : DataType.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_DataType_HH
#define MaslGen_DataType_HH

#include "iumlapi/XUML_DataType.hh"
#include "TagSet.hh"

namespace MaslGen
{
  class DataType
  {
    public:
      DataType ( const iUMLAPI::XUML::DataType& dataType );

      const std::string& getName() const { return name; }
      const std::string& getInternalName() const { return dataType.getDataTypeName(); }
      const std::string& getConstraint() const { return constraint; }
      std::string getModFileFullDefinition ( const std::string& indent, std::set<std::string>& alreadyDefined, bool forcePublic = false ) const;
      const std::vector<DataType>& getContainedTypes() const;
      bool isPublic() const;

      const TagSet& getTagSet() const { return tags; }

    private:
      iUMLAPI::XUML::DataType dataType;
      TagSet tags;
      mutable std::vector<DataType> containedTypes;
      mutable bool ctInitialised;
      std::string name;
      std::string constraint;
      std::string getModFileDefinition( const std::string& indent, bool forcePublic ) const;
      static const std::string& visibilityTag();
  };

}
#endif
