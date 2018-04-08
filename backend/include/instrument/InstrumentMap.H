#ifndef InstrumentMap_H
#define InstrumentMap_H

#include <map>
#include <string>

namespace Algo
{
  class InstrumentMap
  {
  public:
    static InstrumentMap& getInstance();
    int getSymbolId(const std::string&);
    std::map<std::string, int> getAllInstruments();
    void pushSymbol(const int&, const std::string&);

  private:
    std::map<std::string, int> _instrumentMap;
  };
}

#endif
