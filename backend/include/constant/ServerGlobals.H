#ifndef ServerGlobals_H
#define ServerGlobals_H

#include <string>

namespace Algo
{
  const std::string ConfigFile = "./AlgoEngine.conf";
  extern bool gIsRunning;
  const int maxOrdersPerSecond = 10000;
  const int digitsForSeconds = 100000;
}

#endif
