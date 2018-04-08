#include <algorithm>
#include <iterator>

#include <market_data/MarketDataHandler.H>
#include <instrument/InstrumentMap.H>
#include <market_data/ContractsFactory.H>
#include <connection/DatabaseConnection.H>

#include <ib_api/client/EClientSocket.h>
#include <iostream>
#include <ib_api/client/EReader.h>

#ifdef IB_PAPER_ACCOUNT
# define ALGO_IB_BID DELAYED_BID
# define ALGO_IB_ASK DELAYED_ASK
#else
# define ALGO_IB_BID BID
# define ALGO_IB_ASK ASK
#endif

namespace Algo
{
namespace Ib
{

MarketDataHandler::MarketDataHandler() : _isFeedConnected(true)
{}

MarketDataHandler::~MarketDataHandler() {}

bool MarketDataHandler::connect(const std::string& host, const int port)
{
  bool status = ConnectionHandler::connect(host, port);
  return status;
}

bool MarketDataHandler::unsubscribeImplementation(const std::string& symbol) {
}

bool MarketDataHandler::subscribeImplementation(const std::string& symbol, int id) {
  if (this->isConnected()) 
  {
    #ifdef IB_PAPER_ACCOUNT
    // Switch to live (1) frozen (2) delayed (3) or delayed frozen (4)
    _client->reqMarketDataType(3);
    #else
    _client->reqMarketDataType(1);
    #endif

    ::Contract ibContract = ContractsFactory::fromString(symbol);
    std::cout << ibContract.secType << " " << ibContract.currency 
              << " " << ibContract.exchange << " " << ibContract.localSymbol;

    _client->reqMktData(id, ibContract, "", false, TagValueListSPtr());
    std::lock_guard<std::mutex> guard(_mutex);
    _tickerIdToContract[id] = symbol;

    std::cout << "[IB MD] Subscribed to " << symbol << " tickerId " << id;

    return true; 
  }
  else 
  {
    std::cerr << "[IB MD] Not Subscribed to " << symbol << " since Feed connection is not yet ready.";
  }
}

void MarketDataHandler::tickPrice(TickerId tickerId, TickType field,
                                  double price, int canAutoExecute) {
  if (this->isConnected()) {
    std::string symbol = _tickerIdToContract[tickerId];

    switch (field) {
      case ALGO_IB_BID:
        {
          if (price < 0) {
            std::cout << "[IB MD] Ticker " << symbol
                      << " receiving negative ticker price " << price << " ignoring.";
            break;
          }
          Db::Quote quote(tickerId, price, "BID");
          saveQuote(quote);
          std::cout << "[IB MD] Ticker " << symbol << " BID price: " << price;
          break;
        }

      case ALGO_IB_ASK:
        {
          if (price < 0) {
            std::cout << "[IB MD] Ticker " << symbol
                      << " receiving negative ticker price " << price << " ignoring.";
            break;
          }
          Db::Quote quote(tickerId, price, "ASK");
          saveQuote(quote);
          std::cout << "[IB MD] Ticker " << symbol << " ASK price: " << price;
          break;
        }

      default:
        std::cout << "[IB MD] Ticker " << symbol
                  << " skipping recieved ticker type " << field;
    }
  }
  else {
    std::cerr << "[IB MD] Ignoring prices since connection down.";
  }
}

void MarketDataHandler::saveQuote(Db::Quote& quote)
{
  Algo::DatabaseConnection &db = Algo::DatabaseConnection::getInstance();

  db.persistInOneTransaction(quote);
}


void MarketDataHandler::error(const int id, const int errorCode,
                              const std::string errorStr) {}
}
}
