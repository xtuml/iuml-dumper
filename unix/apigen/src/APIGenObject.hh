// 
// Filename : APIGenObject.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef APIGenObject_HH
#define APIGenObject_HH

#include "APIGenAttribute.hh"
#include "APIGenRelationship.hh"

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

namespace APIGen
{
  class Domain;

  class Object
  {
    public: 
      Object ( const std::string& parentDomain, const std::string& line );
      void addAttribute ( const Attribute& attribute )
      {
        attributes.insert(std::pair<std::string, Attribute>(attribute.getName(),attribute));
      }
      void addRelationship ( const Relationship& relationship )
      {
        relationships.insert(std::pair<const std::string,Relationship>(relationship.getName(),relationship));
      }

      const std::string& getName() const { return name; }
      const std::string& getNumber() const { return number; }
      const std::string& getKeyLetters() const { return keyLetters; }
      const std::map<std::string,Attribute>& getAttributes() const { return attributes; }
      const std::multimap<std::string,Relationship>& getRelationships() const { return relationships; }
      std::multimap<std::string,Relationship>& getRelationships() { return relationships; }
      void generateAPI( const std::string& outputdir );
      bool isAssociativeOnly() const;
      void stripAssocOnlyRels( const std::set<std::string>& assocOnlyObjs );
      void addCounterpart( const std::string& toDomain, const std::string& toObject )
      {
        counterparts.push_back(std::pair<std::string,std::string>(toDomain,toObject));
      }

      void addLookup( const std::string& rel, const std::string& att )
      {
        lookups.push_back(std::pair<std::string,std::string>(rel,att));
      }

      void setApiDeleteFn ( const std::string& apiDeleteFn_ )
      {
        this->apiDeleteFn = apiDeleteFn_; 
      }

      struct ApiCreateFunction
      {
        std::string obj;
        std::string function;
        std::vector<std::string> params;
      };

      void addApiCreateFn ( const ApiCreateFunction& apiCreateFunction )
      {
        apiCreateFns.push_back(apiCreateFunction); 
      }


    public:
      const std::string& getFileName() const        { return fileName; }
      const std::string& getClassName() const       { return className; }
      const std::string& getAPINamespace() const    { return apiNamespace; }
      const std::string& getDomainNamespace() const { return domainNamespace; }
      const std::string& getFullClassName() const   { return fullClassName; }

      bool operator< ( const Object& rhs ) const
      { 
        return parentDomain < rhs.parentDomain || 
              ( parentDomain == rhs.parentDomain && 
                name < rhs.name );
      }

    private:
      std::string className;
      std::string fileName;
      std::string apiNamespace;
      std::string domainNamespace;
      std::string fullClassName;

      const Domain& getParentDomain() const;
      void generateAttributeAPI();
      void generateRelationshipAPI();
      void generateCounterpartAPI();
      void generateLookupAPI();
      void generateWriteAPI();
      void generateCreateAPI();
      void generateLinkAPI();
      void generateEditAPI();
      void generateDeleteAPI();
      void generateExtrasAPI();
      void generateReferences();

    private:
      std::string name;
      std::string number;
      std::string keyLetters;
      std::string parentDomain;

      std::map<std::string,Attribute> attributes;
      std::multimap<std::string,Relationship> relationships;
      std::vector<std::pair<std::string,std::string> > counterparts;
      std::vector<std::pair<std::string,std::string> > lookups;
      std::vector<ApiCreateFunction> apiCreateFns;
      std::string apiDeleteFn;
      std::set<Object> referencedObjects;

    private:
      std::string getterDecs;
      std::string getterDefs;
      std::string setterDecs;
      std::string setterDefs;
      std::string writeDecs;
      std::string writeDefs;
      std::string extrasDecs;
      std::string extrasDefs;
      std::string extrasHeader;
      std::string members;
      std::string resetBody;
      std::string forwardDecs;
      std::string includes;
      std::string setGetterDecs;
      std::string setMembers;
      std::string setGetterDefs;

  };

};
#endif
