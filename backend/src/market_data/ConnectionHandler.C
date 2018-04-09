#include <market_data/ConnectionHandler.H>

#include <ib_api/client/EWrapper.h>
#include <ib_api/client/EReader.h>
#include <ib_api/client/EClientSocket.h>
#include <iostream>

namespace Algo
{
namespace Ib
{

ConnectionHandler::ConnectionHandler()
    : _osSignal(2000),  // 2-seconds timeout
      _client(new EClientSocket(this, &_osSignal)),
      _reader(nullptr) {}

ConnectionHandler::~ConnectionHandler() {
  if (_reader) {
    delete _reader;
  }

  delete _client;
}

bool ConnectionHandler::connect(const std::string& host, const int port) {
  static int sClientId = 1;

  std::cout << "Connecting to " << host << ":" << port << std::endl;

  // Each running connection to IB Gateway should have unique client id
  // Basically we will have just two ids for Market data connection and orders.
  bool status = _client->eConnect(host.c_str(), port, sClientId);
  if (!status) {
    std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
    return false;
  }
  std::cout << "Connected to " << host << ":" << port << std::endl;

  ++sClientId;

  _reader = new EReader(_client, &_osSignal);
  _reader->start();

  return status;
}

void ConnectionHandler::disconnect() const
{
  _client->eDisconnect();
  std::cout << "Disconnect" << std::endl;
}

bool ConnectionHandler::isConnected() const
{
  return _client->isConnected();
}

void ConnectionHandler::processMessages()
{
  _reader->checkClient();
  _osSignal.waitForSignal();
  _reader->processMsgs();
}

void ConnectionHandler::error(const int id, const int errorCode,
                              const std::string errorStr) {
  std::cerr << "Id: " << id << " Code: " << errorCode << " Msg: " << errorStr << std::endl;
}

}
}
