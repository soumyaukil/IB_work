#include <market_data/MarketDataHandler.H>
#include <thread>
#include <connection/DatabaseConnection.H>
#include <instrument/InstrumentMap.H>
#include <vector>
#include <db/Instrument_odb.H>
#include <config/Configuration.H>
#include <constant/ServerGlobals.H>
#include <chrono>


bool loadInstruments(Algo::DatabaseConnection &dbConnection_, std::string &error_);
int runServer();

int main()
{
  runServer();
}

int runServer()
{
  Algo::Configuration* config = Algo::Configuration::getInstance();
  config->loadConfig();

  Algo::Ib::MarketDataHandler ibMsgHandler;
  std::string error;
  
  Algo::DatabaseConnection &db = Algo::DatabaseConnection::getInstance();
  bool dbConnected = db.startConnection(
    config->getCentralDbUser(),
    config->getCentralDbPassword(),
    config->getCentralDbName(),
    config->getCentralDbAddress(),
    config->getCentralDbPort()
  );
  if (!dbConnected) {
    return EXIT_FAILURE;
  }
  // load symbols now from db
  if(!loadInstruments(db, error)) {
    std::cerr << "Failed to load instruments from db. Error=[" << error << "] Existing..." << std::endl;
    return EXIT_FAILURE;
  }


  bool connected = ibMsgHandler.connect(config->getIbGatewayHost(), config->getIbGatewayPort());

  if (connected) {
    std::map<std::string, int>::iterator it;
    std::map<std::string, int> instrumentMap = Algo::InstrumentMap::getInstance().getAllInstruments();
    for (it = instrumentMap.begin(); it != instrumentMap.end(); it++)
    {
      std::cout << it->first << std::endl;
      ibMsgHandler.subscribeImplementation(it->first, it->second);
    }
  }
  else {
    std::cerr << "Failed to connect to TWS. Exiting..." << std::endl;
    return EXIT_FAILURE;
  }

  std::thread ibMarketDataThread([&]() {
     while(Algo::gIsRunning) { ibMsgHandler.processMessages(); }
  });

  ibMarketDataThread.detach();
  while(Algo::gIsRunning)
  {
    //let main thread sleep here
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
  return EXIT_SUCCESS;
}

bool loadInstruments(Algo::DatabaseConnection &dbConnection_, std::string &error_)
{
  std::vector<Algo::Db::Instrument> result = dbConnection_.queryAll<Algo::Db::Instrument>();

  for (auto it = result.begin(); it != result.end(); ++it)
  {
     std::cout << "Instrument: (" << it->id() << "," << *it << ")" << std::endl;
     Algo::InstrumentMap::getInstance().pushSymbol(it->id(), *it);
  }
  return true;
}
