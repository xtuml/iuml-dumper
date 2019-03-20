//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
      public:
        void acquireLock();

        long getLatestBuildSetNo() const;

        void freeze(FreezeType type);

      private:
        class BuildSetNoLookup
        {
          public:
            BuildSetNoLookup();
            bool initialised;
            long latestBuildSet;
            int resetCount;
        };

        void  initBuildSetNoLookup() const;
        mutable BuildSetNoLookup buildSetNoLookup;

        // Lock needs to be wrapped in a class because can't 
        // initialise it in the auto-generated constructors. 
        class Lock
        {
          public:
            Lock() : locked(false) {}
            bool locked;
        };
        Lock lock;
