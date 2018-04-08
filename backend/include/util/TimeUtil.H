#ifndef TimeUtil_H
#define TimeUtil_H

#include <boost/date_time/local_time/local_time.hpp>

namespace bt = boost::posix_time;

namespace Algo
{
  namespace TimeUtil
  {
    bt::ptime getCurrentTime();
  }
}
#endif
