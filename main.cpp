
#include <iostream>
#include <assert.h>
#include "DataStore.h"

using namespace std;


int main() {
   cout << "Test suite 1" << endl << endl;

   if (true)
   {
      int theData;
      LRU_Cache<int, int> theCache(3);

      theCache.put(0, 100);
      theCache.put(1, 101);
      theCache.put(2, 102);
      theCache.get(0, theData);
      theCache.put(3, 103);
      theCache.put(3, 203);

      assert(theCache.get(0, theData) == theCache.NO_ERROR);
      assert(theCache.get(1, theData) == theCache.NOT_FOUND);
      assert(theCache.get(2, theData) == theCache.NO_ERROR);
      assert(theCache.get(3, theData) == theCache.NO_ERROR);
      assert(theData == 203);
      assert(theCache.get(4, theData) == theCache.NOT_FOUND);

      theCache.put(4, 104);
      assert(theCache.get(4, theData) == theCache.NO_ERROR);
      theCache.erase(4);
      assert(theCache.get(4, theData) == theCache.NOT_FOUND);
   }

   cout << endl << endl << "Test suite 2" << endl << endl;

   if (true)
   {
      string theData;
      LRU_Cache<string, string> theCache(3);

      theCache.put("0", "100");
      theCache.put("1", "101");
      theCache.put("2", "102");
      theCache.get("0", theData);
      theCache.put("3", "103");

      assert(theCache.get("0", theData) == theCache.NO_ERROR);
      assert(theCache.get("1", theData) == theCache.NOT_FOUND);
      assert(theCache.get("2", theData) == theCache.NO_ERROR);
      assert(theCache.get("3", theData) == theCache.NO_ERROR);
      assert(theCache.get("4", theData) == theCache.NOT_FOUND);
   }

   cout << endl << endl << "Test suite 3" << endl << endl;

   if (true)
   {
      string theData;
      DataStore theDataStore;

      theDataStore.init("test.db");
      theDataStore.clear();

      theDataStore.put("0", "100");
      theDataStore.put("1", "101");
      theDataStore.put("2", "102");
      theDataStore.get("0", theData);
      theDataStore.put("3", "103");
      theDataStore.put("3", "203");

      assert(theDataStore.get("0", theData) == theDataStore.NO_ERROR);
      assert(theDataStore.get("1", theData) == theDataStore.NO_ERROR);
      assert(theDataStore.get("2", theData) == theDataStore.NO_ERROR);
      assert(theDataStore.get("3", theData) == theDataStore.NO_ERROR);
      assert(theData == "203");
      assert(theDataStore.get("4", theData) == theDataStore.NOT_FOUND);

      theDataStore.put("4", "104");
      assert(theDataStore.get("4", theData) == theDataStore.NO_ERROR);
      theDataStore.erase("4");
      assert(theDataStore.get("4", theData) == theDataStore.NOT_FOUND);
   }
}

