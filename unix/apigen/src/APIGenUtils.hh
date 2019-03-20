// 
// Filename : APIGenUtils.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef APIGenUtils_HH
#define APIGenUtils_HH

#include <string>

namespace APIGen
{
  std::string camelCase ( const std::string& value, bool capitaliseFirst, bool lowerFirst = false );
}

#endif
