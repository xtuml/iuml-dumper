// 
// Filename : Scenario.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "Scenario.hh"
#include "TextUtils.hh"

#include "boost/lexical_cast.hpp"
#include "iumlapi/Utils.hh"
#include "iumlapi/XUML_AnalysisArea.hh"

// These should just be static members, but there appears to 
// be a bug in the sun linker/compiler, which crashes the 
// process when the member is accessed via a shared library. 
const std::string& MaslGen::Scenario::nativeTag()  { static std::string t ="native"; return t; }

MaslGen::Scenario::Scenario ( const iUMLAPI::XUML::InitialisationSegment& initSeg )
  : initSeg(initSeg),
    tags(initSeg.getTAGS_TaggedItem())
{
}

bool MaslGen::Scenario::isNative() const
{
  return getTagSet().hasTag(nativeTag());
}

std::string MaslGen::Scenario::getFileNameExtension() const
{
  return "scn";
}

std::string MaslGen::Scenario::getDescription() const
{
  return initSeg.getName();
}

std::string MaslGen::Scenario::getDomainName() const
{
  return iUMLAPI::getDomainKeyLetter(initSeg.getIsDefinedInAnalysisAreaR233());
}

std::string MaslGen::Scenario::getCallableType() const
{
  return "private service";
}

std::string MaslGen::Scenario::getPragmas() const
{
  std::string result = " pragma scenario (" + boost::lexical_cast<std::string>(initSeg.getNumber()) + ");";

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

std::string MaslGen::Scenario::getUnqualifiedName() const
{
  //return makeIdentifier(initSeg.getName()) + "_" + boost::lexical_cast<std::string>(initSeg.getNumber());
  return makeIdentifier(initSeg.getName());
}

std::string MaslGen::Scenario::getMethod() const
{
  return initSeg.getMethod();
}

void MaslGen::Scenario::updateMethod(const std::string& method)
{
  initSeg.setMethod(method);
}

std::string MaslGen::Scenario::getParameterList() const
{
  return "()";
}
