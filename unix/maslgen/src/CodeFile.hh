// 
// Filename : CodeFile.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_CodeFile_HH
#define MaslGen_CodeFile_HH

#include <string>
#include <set>

namespace MaslGen
{
  class CodeFile
  {
    public:
      CodeFile();

      virtual ~CodeFile() {}

      virtual std::string getFileName() const = 0;
      virtual std::string getDescription() const = 0;
      virtual std::string getFileContents() const = 0;
      virtual std::string getCopyrightPrefix() const = 0;
      virtual bool updateModel(const std::string& fileContents);
      virtual bool hasCodeFile() const { return true; }

      void dumpToFile() const;
      void updateFromFile();
      bool okToDump() const;

      std::string getCopyrightNotice() const;

      static void setFilesToDump ( const std::set<std::string>& fileNames ); 

  };
}

#endif
