#include <util/TimeUtil.H>
#include <config/Configuration.H>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace Algo
{
  namespace TimeUtil
  {
    bt::ptime getCurrentTime()
    {
      bt::ptime now = boost::posix_time::microsec_clock::universal_time();
      return now;
    }
  }
}
