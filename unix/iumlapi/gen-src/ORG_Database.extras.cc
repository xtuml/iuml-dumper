//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
void iUMLAPI::ORG::Database::acquireLock()
{
  if ( !lock.locked )
  {
    API_ENTITY editable;
    iUMLAPI::acquire_top_level_lock(entity.getEntity(),editable);
    entity = editable;
    iUMLAPI::release_entity(editable);
    reset();
    lock.locked = true;
  }
}


