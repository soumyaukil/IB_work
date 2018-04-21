#include <util/UniqueIdGenerator.H>
#include <constant/ServerGlobals.H>

namespace Algo
{
  UniqueIdGenerator& UniqueIdGenerator::getInstance()
  {
    static UniqueIdGenerator uniqueIdGenerator;
    return uniqueIdGenerator;
  }

  long UniqueIdGenerator::generateUniqueOrderId()
  {
    // PASSED param is ignored, but kept there so not to change signature
    static int orderIdCounter = 1;
    long orderId = 0;
    {
      std::lock_guard<std::mutex> lock(uniqueOrderIdMutex);
      orderId = std::time(nullptr) % digitsForSeconds + orderIdCounter * digitsForSeconds;
      orderIdCounter++;
      if (orderIdCounter == maxOrdersPerSecond) {
        orderIdCounter = 0;
      }
    }
    return orderId;
  }

  UniqueIdGenerator::UniqueIdGenerator()
  {
  }
}
