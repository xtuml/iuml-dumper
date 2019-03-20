// 
// Filename : SupplementaryCodeBlock.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_SupplementaryCodeBlock_HH
#define MaslGen_SupplementaryCodeBlock_HH

#include <string>
#include <fstream>

#include "CodeFile.hh"

#include "iumlapi/XUML_SupplementaryCodeBlock.hh"
#include "iumlapi/BS_SupplementaryCodeBlock.hh"

namespace MaslGen
{
  class SupplementaryCodeBlock : public CodeFile
  {
    public:
      SupplementaryCodeBlock ( const iUMLAPI::XUML::SupplementaryCodeBlock& scb );
      SupplementaryCodeBlock ( const iUMLAPI::BS::SupplementaryCodeBlock& scb );

      std::string getFileName() const { return filename; }
      std::string getFileContents() const;
      std::string getDescription() const { return ""; }
      std::string getCopyrightPrefix() const { return ""; }
      virtual bool updateModel(const std::string& fileContents);

    private:
      iUMLAPI::XUML::SupplementaryCodeBlock domainScb;
      iUMLAPI::BS::SupplementaryCodeBlock projectScb;
      std::string filename;
  };
}

#endif
