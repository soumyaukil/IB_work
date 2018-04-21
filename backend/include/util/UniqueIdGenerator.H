#ifndef UniqueIdGenerator_H
#define UniqueIdGenerator_H

#include <thread>
#include <mutex>
#include <ctime>

namespace Algo
{
  class UniqueIdGenerator
  {
    public:
      static UniqueIdGenerator& getInstance();
      long generateUniqueOrderId();

    private:
      UniqueIdGenerator();
      std::mutex uniqueOrderIdMutex;
  };
}
#endif
