// 
// Filename : DumpOptions.cc
//
// UK Crown Copyright (c) 2009. All Rights Reserved
//
#include "DumpOptions.hh"
#include "boost/bind.hpp"

namespace MaslGen
{

DumpOptions& DumpOptions::instance()
{
   static DumpOptions instance;
   return instance;
}

DumpOptions::DumpOptions()
{

}

DumpOptions::~DumpOptions()
{

}

std::string DumpOptions::getUsage()
{
   return " *<-no-relships> ";
}

std::vector<std::string> DumpOptions::stripOptions(const std::vector<std::string>& optionList)
{
  std::vector<std::string> ignoredOptions;
  std::for_each(optionList.begin(),optionList.end(), boost::bind(&DumpOptions::addOption,boost::ref(*this),_1,boost::ref(ignoredOptions)));
  return ignoredOptions;
}

void DumpOptions::addOption(const std::string& option, std::vector<std::string>& ignoredOptions)
{
   if (option.size() > 0 && option[0] == '-'){
       options.push_back(option);
   }
   else{
     ignoredOptions.push_back(option);
   }
}

bool DumpOptions::hasNoRelShips()
{
   return std::find(options.begin(),options.end(),std::string("-no-relships")) != options.end();
}

}
