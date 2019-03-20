// 
// Filename : ProjectDomain.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_ProjectDomain_HH
#define MaslGen_ProjectDomain_HH

#include "iumlapi/ORG_Database.hh"
#include "iumlapi/BS_DomainVersionInBuildSet.hh"

#include "ProjectTerminator.hh"

#include "CodeFile.hh"

namespace MaslGen
{
  class ProjectDomain
  {
    public:
      ProjectDomain ( const iUMLAPI::BS::DomainVersionInBuildSet& domainVersion, const iUMLAPI::ORG::Database& database );

      void dumpToFiles() const;
      void updateFromFiles();
      std::string getPrjFileDeclaration(const std::string& indent) const;

    private:
      iUMLAPI::BS::DomainVersionInBuildSet domainVersion;

      mutable std::vector<ProjectTerminator>     terminators;
  };
}

#endif
