#ifndef Configuration_H
#define Configuration_H

#include <string>
#include <mutex>
#include <boost/date_time/local_time/local_time.hpp>

namespace Algo
{
  class Configuration
  {
    public:
      static Configuration *getInstance ();

      const std::string& getIbGatewayHost() const;
      int getIbGatewayPort() const;
      const std::string& getCentralDbAddress() const;
      int getCentralDbPort() const;
      const std::string& getCentralDbUser() const;
      const std::string& getCentralDbPassword() const;
      const std::string& getCentralDbName() const;

      void loadConfig();
      void print();

    private:
      std::string _ibGatewayHost;
      int         _ibGatewayPort;

      std::string _centralDbAddress;
      int         _centralDbPort;
      std::string _centralDbUser;
      std::string _centralDbPassword;
      std::string _centralDbName;
  };
}

#endif
