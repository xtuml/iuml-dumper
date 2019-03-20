// 
// Filename : Session.hh
//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
#ifndef iUMLAPI_Session_HH
#define iUMLAPI_Session_HH

#include "iumlapi/ORG_Database.hh"

#include <map>

namespace iUMLAPI
{

  class Session
  {
    public:
      Session ( const std::string& name, bool writeable = false );

      void abortOnExit()  { databaseSession.abortOnExit(); }
      void commitOnExit() { databaseSession.commitOnExit(); } 

      const ORG::Database& getDatabase() const;

    private:
      class DatabaseSession
      {
        public:
          DatabaseSession();
          ~DatabaseSession();

          Entity logon(const std::string& name, bool writeable);

          void commitOnExit() { if ( status == UpdateAbort ) status = UpdateCommit; }
          void abortOnExit()  { if ( status == UpdateCommit ) status = UpdateAbort; }

        private:
          enum Status { LoggedOff, ReadOnly, UpdateCommit, UpdateAbort };
          Status status;
      };
    
    private:
      // Don't allow copying of sessions, or it will cause havoc 
      // with the database sessions 
      Session(const Session& session);
      Session& operator= (const Session& session);

    private:
      // Database Session must be first member to ensure it is 
      // initialised first and destructed last 
      DatabaseSession databaseSession;

      ORG::Database database;
      bool lockAcquired;
  };


};

#endif
