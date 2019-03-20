// 
// Filename : Domain.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_Domain_HH
#define MaslGen_Domain_HH

#include "iumlapi/ORG_DomainVersion.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

#include "DomainService.hh"         
#include "SupplementaryCodeBlock.hh"
#include "Scenario.hh"              
#include "External.hh"              
#include "DataType.hh"              
#include "Object.hh"                
#include "Bridge.hh"                
#include "CodeFile.hh"                

#include <set>

namespace MaslGen
{
  class Domain : public CodeFile
  {
    public:
      class Interface : public CodeFile
      {
        public:
          Interface ( const Domain& domain );

          std::string getFileContents() const;
          std::string getFileName() const;
          std::string getDescription() const;
          std::string getCopyrightPrefix() const;

        private:
          const Domain& domain;
      };

      Domain ( const iUMLAPI::ORG::DomainVersion& domainVersion );

      void dumpToFiles() const;
      void updateFromFiles();

      std::string getFileContents() const;
      std::string getFileName() const;
      std::string getDescription() const;
      std::string getCopyrightPrefix() const;
    
      const std::string& getKeyLetter() const;
      std::string getDeclaration() const;
      std::string getInterfaceSection() const;
      std::string getSignOff() const;

    private:
      iUMLAPI::XUML::AnalysisArea analysis;
    
      void initCache() const;
      mutable bool cacheInitialised;
      mutable std::vector<DomainService>          domainServices;
      mutable std::vector<SupplementaryCodeBlock> scbs;
      mutable std::vector<Scenario>               scenarios;
      mutable std::vector<External>               externals;
      mutable std::vector<Object>                 objects;
      mutable std::vector<Bridge>                 bridges;
      mutable std::vector<DataType>               types;

      mutable std::string interfaceSection;
      mutable bool interfaceSectionInitialised;

  };
}


#endif
