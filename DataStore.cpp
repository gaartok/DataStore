#include "DataStore.h"
#include <iostream>
#include <sstream>
#include "sqlite3.h"

using namespace std;


DataStore::DataStore()
{
   dataStorage = NULL;
   theCache = new LRU_Cache<string, string>(3);
}



DataStore::~DataStore()
{
   sqlite3_close(dataStorage);
}


int DataStore::init(const char *fileName)
{
   int      rc;
   char *   errMsg = 0;
   char     sqlStr[1024];

   rc = sqlite3_open(fileName, &dataStorage);
   if (rc)
   {
      cout << "Error opening data file '" << fileName << "'" << endl;
      sqlite3_close(dataStorage);
      return SQL_ERROR;
   }

   sprintf_s(sqlStr, sizeof(sqlStr) - 1, "CREATE TABLE IF NOT EXISTS StoredData ("
                                         "key TEXT PRIMARY KEY UNIQUE,"
                                         "data TEXT"
                                         ")");
   rc = sqlite3_exec(dataStorage, sqlStr, NULL, 0, &errMsg);
   if (rc != SQLITE_OK)
   {
      cout << "SQL error during CREATE TABLE MetaData: " << errMsg << endl;
      sqlite3_free(errMsg);
      sqlite3_close(dataStorage);
      dataStorage = NULL;
      return SQL_ERROR;
   }

   return NO_ERROR;
}



int DataStore::clear()
{
   int   rc;
   char* errMsg = 0;

   rc = sqlite3_exec(dataStorage, "DELETE FROM StoredData", NULL, NULL, &errMsg);
   if (rc != SQLITE_OK)
   {
      cout << "SQL error during DELETE FROM StoredData: " << errMsg << endl;
      sqlite3_free(errMsg);
      return SQL_ERROR;
   }

   return NO_ERROR;
}



void DataStore::erase(std::string theKey)
{
   int   rc;
   char* errMsg = 0;
   ostringstream stringStream;

   theCache->erase(theKey);

   stringStream << "DELETE FROM StoredData WHERE key='" << theKey << "'";
   cout << stringStream.str() << endl;

   rc = sqlite3_exec(dataStorage, stringStream.str().c_str(), NULL, NULL, &errMsg);
   if (rc != SQLITE_OK)
   {
      cout << "SQL error during DELETE FROM StoredData: " << errMsg << endl;
      sqlite3_free(errMsg);
   }
}



int DataStore::get(string theKey, string& theData)
{
   int errorCode = theCache->get(theKey, theData);
   if (!errorCode) 
      return NO_ERROR;   // Cache hit

   errorCode = getFromDB(theKey, theData);
   return errorCode;
}


int DataStore::getFromDB(string theKey, string& theData)
{
   int errorCode = 0;
   sqlite3_stmt* stmt;
   ostringstream stringStream;

   stringStream << "SELECT data FROM StoredData WHERE key='" << theKey << "'";
   cout << stringStream.str() << endl;
   int rc = sqlite3_prepare_v2(dataStorage, stringStream.str().c_str(), -1, &stmt, NULL);
   if (rc != SQLITE_OK)
   {
      cout << "SQL error during SELECT data: " << sqlite3_errmsg(dataStorage) << endl;
      return SQL_ERROR;
   }
   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW)
   {
      const unsigned char* name = sqlite3_column_text(stmt, 0);
      theData = (char*)name;
      cout << "theData = '" << theData << "'" << endl;
      theCache->put(theKey, theData);
      rc = sqlite3_step(stmt);
   }
   else
   {
      errorCode = NOT_FOUND;
      cout << "data for key '" << theKey << "' is not in DataStore" << endl;
   }

   if (rc != SQLITE_DONE)
   {
      cout << "SQL error during SELECT data: " << sqlite3_errmsg(dataStorage) << endl;
      errorCode = SQL_ERROR;
   }
   sqlite3_finalize(stmt);

   return errorCode;
}



int DataStore::put(string theKey, string theData)
{
   int    rc;
   int    errorCode;
   char * errMsg = 0;
   string existingData;
   ostringstream stringStream;
   ostringstream foundData;

   errorCode = theCache->get(theKey, existingData);
   if (!errorCode)
   {
      if (existingData == theData)
         return NO_ERROR;  // Cache hit and no change.
   }

   theCache->put(theKey, theData);

   errorCode = getFromDB(theKey, existingData);
   if (!errorCode)
   {
      if (existingData == theData)
      {
         cout << "Found key '" << theKey << "' in the DB with no change" << endl;
         return NO_ERROR;
      }

      // In the DB but has changed
      cout << "Found key '" << theKey << "' in the DB but has changed" << endl;
      stringStream << "UPDATE StoredData set data='" << theData << "' WHERE key='" << theKey << "'";
      cout << stringStream.str() << endl;
      rc = sqlite3_exec(dataStorage, stringStream.str().c_str(), NULL, NULL, &errMsg);
      if (rc != SQLITE_OK)
      {
         cout << "SQL error during UPDATE StoredData: " << errMsg << endl;
         sqlite3_free(errMsg);
         return SQL_ERROR;
      }
      return NO_ERROR;
   }

   // Add new entry to DB
   stringStream << "INSERT INTO StoredData(key,data) "
                << "VALUES ('" << theKey << "','" << theData << "')";

   cout << stringStream.str() << endl;

   rc = sqlite3_exec(dataStorage, stringStream.str().c_str(), NULL, NULL, &errMsg);
   if (rc != SQLITE_OK)
   {
      cout << "SQL error during INSERT INTO StoredData: " << errMsg << endl;
      sqlite3_free(errMsg);
      return SQL_ERROR;
   }

   return NO_ERROR;
}


