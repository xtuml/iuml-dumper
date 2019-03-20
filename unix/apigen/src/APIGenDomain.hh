// 
// Filename : APIGenDomain.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef APIGenDomain_HH
#define APIGenDomain_HH

#include "APIGenObject.hh"

#include <string>
#include <ostream>
#include <map>
#include <set>


namespace APIGen
{

  class Domain
  {
    public: 
      Domain ( const std::string& name );
      void addObject ( const Object& object ) { objects.insert(std::make_pair(object.getKeyLetters(),object)); }

      const std::string& getName() const { return name; }
      const std::map<std::string,Object>& getObjects() const { return objects; }
      const Object& getObject( const std::string& kl ) const;
      Object& getObject( const std::string& kl );


      void generateAPI( const std::string& outputdir );

    private:
      std::string name;
      std::map<std::string,Object> objects;


  };

  class Domains
  {
    public:
      static void addDomain ( const Domain& domain ) { domains.insert(std::make_pair(domain.getName(),domain)); }
      static Domain& getDomain( const std::string& name );
      static std::map<std::string,Domain>& getDomains() { return domains; }
    private:
      static std::map<std::string,Domain> domains;
  };

};
#endif
