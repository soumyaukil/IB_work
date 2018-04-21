#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/version.hpp>
#include <boost/filesystem/config.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/string_file.hpp>

#include <thread>
#include <config/Configuration.H>
#include <constant/ServerGlobals.H>

namespace Algo
{
  namespace bpo = boost::program_options;

  Configuration* Configuration::getInstance()
  {
    static Configuration conf;
    return &conf;
  }

  const std::string& Configuration::getCentralDbAddress() const
  {
    return _centralDbAddress;
  }

  int Configuration::getCentralDbPort() const
  {
    return _centralDbPort;
  }

  const std::string& Configuration::getCentralDbUser() const
  {
    return _centralDbUser;
  }

  const std::string& Configuration::getCentralDbPassword() const
  {
    return _centralDbPassword;
  }

  const std::string& Configuration::getCentralDbName() const
  {
    return _centralDbName;
  }

  const std::string& Configuration::getIbGatewayHost() const {
    return _ibGatewayHost;
  }

  int Configuration::getIbGatewayPort() const {
    return _ibGatewayPort;
  }

  std::string& Configuration::getAccountNumberBuy() {
    return _accountNumberBuy;
  }

  std::string& Configuration::getAccountNumberSell() {
    return _accountNumberSell;
  }

  void Configuration::loadConfig()
  {
    try
    {
      bpo::options_description config("Configuration");
      config.add_options()

      ("COMMON.CentralDbAddress",
        bpo::value<std::string>(&_centralDbAddress)->default_value("127.0.0.1"),
        "Central DB Address")

      ("COMMON.CentralDbPort",
        bpo::value<int>(&_centralDbPort)->default_value(3306),
        "Central DB Port")

      ("COMMON.CentralDbUser",
        bpo::value<std::string>(&_centralDbUser)->default_value("root"),
        "Central DB User")

      ("COMMON.CentralDbPassword",
        bpo::value<std::string>(&_centralDbPassword)->default_value("q1w2e3"),
        "Central DB Password")

      ("COMMON.CentralDbName",
        bpo::value<std::string>(&_centralDbName)->default_value("AlgoEngine"),
        "Central DB Name")

      ("COMMON.AccountNumberBuy",
       bpo::value<std::string>(&_accountNumberBuy)->default_value("1000662639"),
       "Account Number Buy")

      ("COMMON.AccountNumberSell",
       bpo::value<std::string>(&_accountNumberSell)->default_value("1000662639"),
       "Account Number Sell")
      
      ("COMMON.IbApiGatewayHost",
        bpo::value<std::string>(&_ibGatewayHost)->default_value("127.0.0.1"),
        "IB API Gateway host address")

      ("COMMON.IbApiGatewayPort",
        bpo::value<int>(&_ibGatewayPort)->default_value(7497),
        "IB API Gateway host address")
      ;

      bpo::options_description configFileOptions;
      configFileOptions.add(config);

      bpo::variables_map vm;
      std::ifstream ifs(ConfigFile.c_str());

      if (ifs)
      {
        bpo::store(bpo::parse_config_file(ifs, configFileOptions, true), vm);
        bpo::notify(vm);
      }
      else
      {
        std::cout << "File open Error: " << ConfigFile << std::endl;
        return;
      }
    }
    catch (boost::program_options::error &e)
    {
      std::cerr << "Bad Options: " << e.what() << std::endl;
      return;
    }
    catch (std::exception& e)
    {
      std::cerr << " Unknown Exception: " << e.what() << std::endl;
      return;
    }
    catch (...)
    {
      std::cerr << __FUNCTION__ << " : " << __LINE__ << "Unknown Exception: " << std::endl;
      return;
    }
  }
}
