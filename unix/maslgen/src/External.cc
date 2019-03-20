// 
// Filename : External.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "External.hh"
#include "TextUtils.hh"

#include "boost/lexical_cast.hpp"
#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

// These should just be static members, but there appears to 
// be a bug in the sun linker/compiler, which crashes the 
// process when the member is accessed via a shared library. 
const std::string& MaslGen::External::nativeTag()  { static std::string t ="native"; return t; }

MaslGen::External::External ( const iUMLAPI::XUML::TestMethod& test )
  : test(test),
    tags(test.getTAGS_TaggedItem())
{
}

bool MaslGen::External::isNative() const
{
  return getTagSet().hasTag(nativeTag());
}

std::string MaslGen::External::getFileNameExtension() const
{
  return "ext";
}

std::string MaslGen::External::getDescription() const
{
  return test.getName();
}


std::string MaslGen::External::getDomainName() const
{
  return iUMLAPI::getDomainKeyLetter(test.getIsDefinedInAnalysisAreaR227());
}

std::string MaslGen::External::getCallableType() const
{
  return "private service";
}

std::string MaslGen::External::getPragmas() const
{
  std::string result = " pragma external (" + boost::lexical_cast<std::string>(test.getNumber()) + ");";

  for ( std::vector<std::pair<std::string,std::string> >::const_iterator it = getTagSet().getTags().begin();
        it != getTagSet().getTags().end();
        ++it )
  {
    if ( it->first != nativeTag() )
    {
      result+= " pragma " + makeIdentifier(it->first) + " (\"" + it->second + "\");";
    }
  }

  return result;
}

std::string MaslGen::External::getUnqualifiedName() const
{
  return makeIdentifier(test.getName()) + "_" + boost::lexical_cast<std::string>(test.getNumber());
}

std::string MaslGen::External::getMethod() const
{
  return test.getMethod();
}

void MaslGen::External::updateMethod( const std::string& method )
{
  return test.setMethod(method);
}


std::string MaslGen::External::getParameterList() const
{
  return "()";
}
