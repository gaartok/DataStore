#pragma once

#include "LRU_Cache.h"


struct sqlite3;

class DataStore
{
public:
   DataStore();
   ~DataStore();
   int init(const char* fileName);
   int get(std::string theKey, std::string& theData);
   int put(std::string theKey, std::string theData);
   void erase(std::string theKey);
   int clear();

   const int NO_ERROR = 0;
   const int NOT_FOUND = -1;
   const int SQL_ERROR = -2;

private:
   LRU_Cache<std::string, std::string>* theCache;
   int getFromDB(std::string theKey, std::string& theData);
   sqlite3* dataStorage;

};

