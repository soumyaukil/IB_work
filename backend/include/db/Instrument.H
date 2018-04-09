#ifndef Instrument_H
#define Instrument_H

#include <odb/core.hxx>
#include <string>

namespace Algo
{
namespace Db
{
  #pragma db object table("Instruments")
  class Instrument
  {
    public:
      Instrument() {}
      Instrument(const std::string& name,     const std::string& localSymbol_,
                 const std::string& secType,  const std::string& exchange, 
                 const std::string& currency) : _name(name), _localSymbol(localSymbol_) ,
                                                _secType(secType), _exchange(exchange), 
                                                _currency(currency)
                                            {}

      unsigned long id() const { return _id; }
      void setId(unsigned long id) { _id = id; }

      const std::string& name() const { return _name; }
      void setName(const std::string& name) { _name = name; }

      const std::string& localSymbol() const { return _localSymbol; }
      void setLocalSymbol(const std::string& localSymbol) { _localSymbol = localSymbol; }

      const std::string& secType() const { return _secType; }
      void setSecType(const std::string& secType) { _secType = secType; }

      const std::string& exchange() const { return _exchange; }
      void setExchange(const std::string& exchange) { _exchange = exchange; }

      const std::string& currency() const { return _currency; }
      void setCurrency(const std::string& currency) { _currency = currency; }

      bool operator==(const Instrument &rhs) const;
      operator std::string() const;

    private:
      friend class odb::access;

      #pragma db id auto column("Id")
      unsigned long _id;

      #pragma db column("Name")
      std::string _name;

      #pragma db column("LocalSymbol")
      std::string _localSymbol;

      #pragma db column("SecType")
      std::string _secType;

      #pragma db column("Exchange")
      std::string _exchange;

      #pragma db column("Currency")
      std::string _currency;
  };
  std::ostream &operator <<(std::ostream &stream, const Instrument &rhs);
} // namespace Db
} // namespace Algo

#endif
