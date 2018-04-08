#ifndef Quote_H
#define Quote_H

#include <odb/core.hxx>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <util/TimeUtil.H>

namespace Algo
{
namespace Db
{
  #pragma db object table("Quotes")
  class Quote
  {
    public:
      Quote(unsigned long id_, double receivedPrice_,
           std::string type_) : _id(id_),
                               _receivedPrice(receivedPrice_),
                               _type(type_),
                               _timestamp(TimeUtil::getCurrentTime()) {}

      unsigned long id() const { return _id; }
      void setId(unsigned long id) { _id = id; }

      double receivedPrice() const { return _receivedPrice; }
      void setReceivedPrice(double receivedPrice) { _receivedPrice = receivedPrice; }

      std::string type() const { return _type; }
      void setType(const std::string &type_) { _type = type_; }

      boost::posix_time::ptime timestamp() const { return _timestamp; }
      void setTimestamp(boost::posix_time::ptime ts) { _timestamp = ts; }

    private:
      friend class odb::access;

      #pragma db id column("SymbolId")
      unsigned long _id;

      #pragma db column("ReceivedPrice") type("DOUBLE PRECISION(20, 10)")
      double _receivedPrice;

      #pragma db column("Type") type("VARCHAR(10)")
      std::string _type;

      // TIMESTAPMP(6) - 6 digit decimal precision of seconds
      #pragma db column("Timestamp") type("TIMESTAMP(6)") \
        options("DEFAULT CURRENT_TIMESTAMP(6)") not_null
      boost::posix_time::ptime _timestamp;
  };
}
}

#endif
