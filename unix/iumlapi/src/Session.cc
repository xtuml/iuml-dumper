// 
// Filename : Session.cc
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#include "iumlapi/Session.hh"
#include "iumlapi/iUMLAPI.hh"
#include "iumlapi/Entity.hh"

#include <iostream>

#include "boost/lexical_cast.hpp"

iUMLAPI::Session::Session( const std::string& name, bool writeable )
  : databaseSession(),
    database(databaseSession.logon(name,writeable)),
    lockAcquired(false)
{
}

const iUMLAPI::ORG::Database& iUMLAPI::Session::getDatabase() const
{
  return database;
}


iUMLAPI::Session::DatabaseSession::DatabaseSession()
{
}
 
iUMLAPI::Entity iUMLAPI::Session::DatabaseSession::logon ( const std::string& name, bool writeable )
{
  API_ENTITY db;
  if ( writeable )
  {
    iUMLAPI::logon_update(name,db);
    status = UpdateCommit;
  }
  else
  {
    iUMLAPI::logon(name,db);
    status = ReadOnly;
  }
  Entity database(db);
  iUMLAPI::release_entity(db);
  return database;
}

iUMLAPI::Session::DatabaseSession::~DatabaseSession()
{
  switch ( status )
  {
    case LoggedOff: 
      break;
    case ReadOnly: 
      iUMLAPI::logoff();
      break;
    case UpdateAbort: 
      std::cerr << "Aborting Transaction" << std::endl;
      iUMLAPI::logoff_abort_update();
      break;
    case UpdateCommit: 
      std::cerr << "Committing Transaction" << std::endl;
      iUMLAPI::logoff_update();
      break;
  }
}
