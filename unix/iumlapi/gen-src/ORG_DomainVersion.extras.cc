//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
void iUMLAPI::ORG::DomainVersion::acquireLock()
{
  if ( !lock.locked )
  {
    API_ENTITY editable;
    iUMLAPI::acquire_analysis_area_lock(entity.getEntity(),editable);
    entity = editable;
    iUMLAPI::release_entity(editable);
    reset();
    lock.locked = true;
  }
}

void iUMLAPI::ORG::DomainVersion::freeze(FreezeType type)
{
  iUMLAPI::freeze_domain_version(entity.getEntity(),type);
  locked.first = false;
  frozen.first = false;
}


