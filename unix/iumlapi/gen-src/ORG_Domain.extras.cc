//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "boost/lexical_cast.hpp"

iUMLAPI::ORG::Domain::VersionNoLookup::VersionNoLookup()
 : initialised(false),
   latestVersion(0),
   latestLockedVersion(0),
   latestFrozenVersion(0),
   latestUnlockedVersion(0),
   resetCount(0)
{
}

void iUMLAPI::ORG::Domain::initVersionNoLookup() const
{
  if ( resetCount != versionNoLookup.resetCount )
  {
    // There's been a reset, so clear the cache
    versionNoLookup = VersionNoLookup();
  }

  if ( !versionNoLookup.initialised )
  {
    versionNoLookup.initialised = true;
    versionNoLookup.resetCount = resetCount;
    for ( ORG::DomainVersion_Set::const_iterator it = getHasDomainVersionR4().begin();
          it != getHasDomainVersionR4().end();
          ++it )
    {
      versionNoLookup.latestVersion = std::max(versionNoLookup.latestVersion,it->getVersionNumber());
      if ( it->getLocked() )
      {
        versionNoLookup.latestLockedVersion = std::max(versionNoLookup.latestLockedVersion,it->getVersionNumber());
      }
      if ( it->getFrozen() )
      {
        versionNoLookup.latestFrozenVersion = std::max(versionNoLookup.latestFrozenVersion,it->getVersionNumber());
      }
      if ( !it->getFrozen() && !it->getLocked() )
      {
        versionNoLookup.latestUnlockedVersion = std::max(versionNoLookup.latestUnlockedVersion,it->getVersionNumber());
      }
    }
  }
}

long iUMLAPI::ORG::Domain::getLatestVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestVersion;
}

long iUMLAPI::ORG::Domain::getLatestLockedVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestLockedVersion;
}

long iUMLAPI::ORG::Domain::getLatestFrozenVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestFrozenVersion;
}

long iUMLAPI::ORG::Domain::getLatestUnlockedVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestUnlockedVersion;
}

iUMLAPI::ORG::DomainVersion iUMLAPI::ORG::Domain::duplicateVersion ( long sourceVersion, const std::string& newName )
{  
  long newVersion = getLatestVersionNo() + 1;
  return createDomainVersion(sourceVersion,newName,newVersion,"Analysis");
}
