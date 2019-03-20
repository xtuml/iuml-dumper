// 
// Filename : TextUtils.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef MaslGen_TextUtils_HH
#define MaslGen_TextUtils_HH

#include <string>

namespace MaslGen
{

  std::string makeIdentifier ( const std::string& value );
  std::string alignTabs ( const std::string& value );
  std::string indentText ( const std::string& indent, const std::string& value );
  std::string wrapText ( const std::string& value, int maxLineLen );
  std::string wrapLine ( const std::string& value, int maxLineLen );
  std::string wrapDescription ( const std::string& indent, const std::string& description, int maxLineLen );

}
#endif
