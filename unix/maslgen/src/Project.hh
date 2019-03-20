// 
// Filename : Project.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Project_HH
#define MaslGen_Project_HH

#include "iumlapi/ORG_Database.hh"
#include "iumlapi/BS_BuildSet.hh"
#include "iumlapi/ORG_BuildSet.hh"

#include "ProjectDomain.hh"
#include "SupplementaryCodeBlock.hh"

#include "CodeFile.hh"

namespace MaslGen
{
  class Project : public CodeFile
  {
    public:
      Project ( const iUMLAPI::ORG::BuildSet& buildSet, const iUMLAPI::ORG::Database& database );

      void dumpToFiles() const;
      void updateFromFiles();

      std::string getFileName() const;
      std::string getFileContents() const;
      std::string getDescription() const;
      std::string getCopyrightPrefix() const;

    private:
      iUMLAPI::BS::BuildSet buildSet;
      const iUMLAPI::ORG::Database& database;

      void initCache() const;
      mutable bool cacheInitialised;
      mutable std::vector<SupplementaryCodeBlock> scbs;
      mutable std::vector<ProjectDomain>     domains;
  };
}

#endif
