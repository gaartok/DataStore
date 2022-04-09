
# DataStore

A basic, persistant data store using sqlite3 and an LRU cache

There are two components included in this repo, the DataStore, and the LRU_Cache.  

The LRU_Cache may be used on its own, and is a templated class to specify the data and key types.  
The types cannot be mixed in a single instance, but multiple instances may be created to cache different types.

The DataStore class uses the LRU_Cache as well as a sqlite database to persist the data input to it.
It is hard coded to store std::strings, but that should be easy enough to change if needed. 

Note that these classes are not thread safe, and if that is required, code will need to be added to implement that. 

See main.cpp for examples of how to use these classes.
