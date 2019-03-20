//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
      public:
        long getLatestVersionNo() const;
        long getLatestLockedVersionNo() const;
        long getLatestFrozenVersionNo() const;
        long getLatestUnlockedVersionNo() const;

        DomainVersion duplicateVersion ( long sourceVersion, const std::string& name );

      private:
        class VersionNoLookup
        {
          public:
            VersionNoLookup();
            bool initialised;
            long latestVersion;
            long latestLockedVersion;
            long latestFrozenVersion;
            long latestUnlockedVersion;
            int resetCount;
        };

        void  initVersionNoLookup() const;
        mutable VersionNoLookup versionNoLookup;

