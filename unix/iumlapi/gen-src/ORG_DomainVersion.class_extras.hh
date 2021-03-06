//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
        void acquireLock();

        void freeze(FreezeType type);

      private:

        // Lock needs to be wrapped in a class because can't 
        // initialise it in the auto-generated constructors. 
        class Lock
        {
          public:
            Lock() : locked(false) {}
            bool locked;
        };
        Lock lock;
