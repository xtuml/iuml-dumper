// 
// Filename : DumpOptions.hh
//
// UK Crown Copyright (c) 2009. All Rights Reserved
//
#ifndef MaslGen_DumpOptions_HH
#define MaslGen_DumpOptions_HH

#include <vector>
#include <string>

namespace MaslGen
{

class DumpOptions
{
   public:
       static DumpOptions& instance();

       std::vector<std::string> stripOptions(const std::vector<std::string>& optionList);
       std::string getUsage();

       bool hasNoRelShips();

   private:
       DumpOptions();
      ~DumpOptions();
       DumpOptions(const DumpOptions& rhs);
       DumpOptions& operator=(const DumpOptions& rhs);

       void addOption(const std::string& option, std::vector<std::string>& ignoredOptions);

   private:
       std::vector<std::string> options;
};


}

#endif
