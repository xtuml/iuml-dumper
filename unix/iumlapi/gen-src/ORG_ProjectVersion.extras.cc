//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "boost/lexical_cast.hpp"

void iUMLAPI::ORG::ProjectVersion::acquireLock()
{
  if ( !lock.locked )
  {
    API_ENTITY editable;
    iUMLAPI::acquire_project_version_lock(entity.getEntity(),editable);
    entity = editable;
    iUMLAPI::release_entity(editable);
    reset();
    lock.locked = true;
  }
}

iUMLAPI::ORG::ProjectVersion::BuildSetNoLookup::BuildSetNoLookup()
 : initialised(false),
   latestBuildSet(0),
   resetCount(0)
{
}

void iUMLAPI::ORG::ProjectVersion::initBuildSetNoLookup() const
{
  if ( resetCount != buildSetNoLookup.resetCount )
  {
    // There's been a reset, so clear the cache
    buildSetNoLookup = BuildSetNoLookup();
  }

  if ( !buildSetNoLookup.initialised )
  {
    buildSetNoLookup.initialised = true;
    buildSetNoLookup.resetCount = resetCount;
    for ( ORG::BuildSet_Set::const_iterator it = getHasBuildSpecificationDefinedByBuildSetR20().begin();
          it != getHasBuildSpecificationDefinedByBuildSetR20().end();
          ++it )
    {
      buildSetNoLookup.latestBuildSet = std::max(buildSetNoLookup.latestBuildSet,it->getBuildSetNumber());
    }
  }
}

long iUMLAPI::ORG::ProjectVersion::getLatestBuildSetNo() const
{
  initBuildSetNoLookup();
  return buildSetNoLookup.latestBuildSet;
}

void iUMLAPI::ORG::ProjectVersion::freeze(FreezeType type)
{
  iUMLAPI::freeze_project_version(entity.getEntity(),type);
  locked.first = false;
  frozen.first = false;
}



