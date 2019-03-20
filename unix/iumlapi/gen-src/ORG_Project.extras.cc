//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "boost/lexical_cast.hpp"

iUMLAPI::ORG::Project::VersionNoLookup::VersionNoLookup()
 : initialised(false),
   latestVersion(0),
   latestLockedVersion(0),
   latestFrozenVersion(0),
   latestUnlockedVersion(0),
   resetCount(0)
{
}

void iUMLAPI::ORG::Project::initVersionNoLookup() const
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
    for ( ORG::ProjectVersion_Set::const_iterator it = getHasProjectVersionR6().begin();
          it != getHasProjectVersionR6().end();
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

long iUMLAPI::ORG::Project::getLatestVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestVersion;
}

long iUMLAPI::ORG::Project::getLatestLockedVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestLockedVersion;
}

long iUMLAPI::ORG::Project::getLatestFrozenVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestFrozenVersion;
}

long iUMLAPI::ORG::Project::getLatestUnlockedVersionNo() const
{
  initVersionNoLookup();
  return versionNoLookup.latestUnlockedVersion;
}

iUMLAPI::ORG::ProjectVersion iUMLAPI::ORG::Project::duplicateVersion ( long sourceVersion, const std::string& newName )
{
  long newVersion = getLatestVersionNo() + 1;
  return createProjectVersion(sourceVersion,newName,newVersion,"Analysis");
}

