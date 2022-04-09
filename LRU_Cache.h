#pragma once

#include <map>
#include <list>
#include <iostream>


template <typename KEY, typename DATA>
class LRU_Cache {
public:
   LRU_Cache(int maxSize);
   int get(KEY theKey, DATA& theData);
   void put(KEY theKey, DATA theData);
   void erase(KEY theKey);

   const int NO_ERROR = 0;
   const int NOT_FOUND = -1;

private:
   typedef typename std::list<std::pair<DATA, size_t> >::iterator DataListItr;
   typedef typename std::map<size_t, DataListItr>::iterator HashMapItr;

   int maxSize;
   std::hash<KEY> hashFunc;
   std::list<std::pair<DATA, size_t> > dataList;
   std::map<size_t, DataListItr> hashMap;
};


template<typename KEY, typename DATA>
LRU_Cache<KEY, DATA>::LRU_Cache(int maxSizeIn)
{
   //std::cout << "KEY type = " << typeid(KEY).name() << std::endl;
   //std::cout << "DATA type = " << typeid(DATA).name() << std::endl;
   maxSize = maxSizeIn;
}



template<typename KEY, typename DATA>
void LRU_Cache<KEY, DATA>::put(KEY theKey, DATA theData)
{
   std::size_t keyHash = hashFunc(theKey);
   HashMapItr hashMapItr = hashMap.find(keyHash);
   if (hashMapItr != hashMap.end())
   {
      std::cout << "Overwriting old cache data with new: '" << theData << "' in location '" << theKey << "'" << std::endl;
      DataListItr dataListItr = (*hashMapItr).second;
      dataList.erase(dataListItr);
      hashMap.erase(hashMapItr);
      dataList.push_front(std::make_pair(theData, keyHash));
      hashMap.insert({ keyHash, dataList.begin() });
      return;
   }

   if (hashMap.size() >= maxSize)
   {
      std::size_t oldestKeyHash;
      DATA oldestData;

      oldestKeyHash = dataList.back().second;
      oldestData = dataList.back().first;
      std::cout << "Removing oldest item from cache: '" << oldestData << "'" << std::endl;
      HashMapItr hashMapItr = hashMap.find(oldestKeyHash);
      hashMap.erase(hashMapItr);
      dataList.pop_back();
   }

   std::cout << "Putting data '" << theData << "' into location '" << theKey << "'" << std::endl;
   dataList.push_front(std::make_pair(theData, keyHash));
   hashMap.insert({ keyHash, dataList.begin()});
}



template<typename KEY, typename DATA>
int LRU_Cache<KEY, DATA>::get(KEY theKey, DATA &theData)
{
   std::size_t keyHash;

   keyHash = hashFunc(theKey);
   //std::cout << "Looking for data for key '" << theKey << "'" << std::endl;
   HashMapItr hashMapItr = hashMap.find(keyHash);
   if (hashMapItr == hashMap.end())
      return NOT_FOUND;

   DataListItr dataListItr = (*hashMapItr).second;
   theData = (*dataListItr).first;

   std::cout << "Moving cache item '" << theKey << "' to the front" << std::endl;
   dataList.splice(dataList.begin(), dataList, dataListItr);

   //std::cout << "Found data '" << theData << "'" << std::endl;
   return NO_ERROR;
}



template<typename KEY, typename DATA>
void LRU_Cache<KEY, DATA>::erase(KEY theKey)
{
   std::size_t keyHash = hashFunc(theKey);
   HashMapItr hashMapItr = hashMap.find(keyHash);
   if (hashMapItr == hashMap.end())
      return;

   std::cout << "Invalidating data at location '" << theKey << "'" << std::endl;
   DataListItr dataListItr = (*hashMapItr).second;
   dataList.erase(dataListItr);
   hashMap.erase(hashMapItr);
}