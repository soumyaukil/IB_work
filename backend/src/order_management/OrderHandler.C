#include <order_management/OrderHandler.H>
#include <util/FixMessageReader.H>
#include <quickfix/SessionSettings.h>
#include <quickfix/fix44/Logout.h>
#include <quickfix/fix44/Logon.h>

namespace Algo { namespace Ib 
{
  long OrderHandler::_clOrdId;
  FIX::SessionID OrderHandler::_sessioId;
  int OrderHandler::_enableDebugLog;

  void OrderHandler::start(std::string &fixConfigFile, int enableDebugLog)
  {
    _enableDebugLog = enableDebugLog;
    _settings = new FIX::SessionSettings(fixConfigFile, false);
    _storeFactory = new FIX::FileStoreFactory(*_settings);
    _logFactory = new FIX::ScreenLogFactory(*_settings);

    _initiator =
      new FIX::ThreadedSocketInitiator(*this,
          *_storeFactory,
          *_settings, *_logFactory);
    _initiator->start();
  }

  void OrderHandler::stop()
  {
    _initiator->stop();
  }

  bool OrderHandler::isConnected()
  {
    return _isConnected;
  }

  void OrderHandler::setStartClOrderId(long id)
  {
    _clOrdId = id;
  }

  OrderHandler::OrderHandler()
  {
    _clOrdId = 1000;
  }

  void OrderHandler::onCreate(const FIX::SessionID &sessionID)
  {
  }

  void OrderHandler::onLogon(const FIX::SessionID &sessionID)
  {
    std::cout << "[FXCM OP] Logon received" << std::endl;
    setSessionInf(sessionID);
  }

  void OrderHandler::onLogout(const FIX::SessionID &sessionID)
  {
  }

  void OrderHandler::toApp (FIX::Message & message, const FIX::SessionID &) throw (FIX::DoNotSend)
  {
  }

  void OrderHandler::fromApp(const FIX::Message & message, const FIX::SessionID &sessionID) throw (
      FIX::FieldNotFound,
      FIX::IncorrectDataFormat,
      FIX::IncorrectTagValue,
      FIX::UnsupportedMessageType)
  {
    std::cout << "IN : " << message.toString() << std::endl;
    crack(message, sessionID); 
  }

  void OrderHandler::setHeader(FIX::Message & message, const FIX::SessionID & sessionId)
  {
    message.getHeader().setField(FIX::SenderCompID
        (_settings->get(sessionId).getString
         ("SenderCompID")));
    message.getHeader().setField(FIX::TargetCompID
        (_settings->get(sessionId).getString
         ("TargetCompID")));
    message.getHeader().setField(FIX::TargetSubID
        (_settings->
         get(sessionId).getString
         ("TargetSubID")));
  }

  void OrderHandler::toAdmin(FIX::Message & message, const FIX::SessionID & sessionId)
  {
    FIX::MsgType msgType;
    message.getHeader().getField(msgType);

    setHeader(message, sessionId);
    if (((msgType.getValue())) == (FIX::MsgType_Logon)) {
      const FIX::Dictionary & sessionSettings =
        _settings->get(sessionId);
      if (sessionSettings.has("Username")) {
        message.setField(FIX::Username
            (sessionSettings.getString("Username")));
      }
      if (sessionSettings.has("Password")) {
        message.setField(FIX::Password
            (sessionSettings.getString("Password")));
      }
      //message.setField(FIX::Account(_account));

      message.setField(FIX::EncryptMethod(0));
      message.getHeader().setField(FIX::MsgSeqNum(1));
      message.setField(FIX::
          ResetSeqNumFlag(FIX::ResetSeqNumFlag_YES));
      std::cout << "[FXCM OP] Sending logon message s" <<
        message.toString() << std::endl;
    }
  }

  void OrderHandler::fromAdmin (const FIX::Message &message, const FIX::SessionID&) throw(
      FIX::FieldNotFound,
      FIX::IncorrectDataFormat,
      FIX::IncorrectTagValue,
      FIX::RejectLogon)
  {
    if (_enableDebugLog)
    {
      FixMessageReader::deserialize("INBOUND [FXCM OP]", message.toString ());
    }
  }

  void OrderHandler::onMessage(const FIX44::Logout &message, const FIX::SessionID &sessionID)
  {
    std::cout << "[FXCM OP] Logout received" << std::endl;
    _isConnected = false;
  }

  void OrderHandler::onMessage(const FIX44::Logon &message,const FIX::SessionID &sessionID)
  {
    if (_enableDebugLog) {
      FixMessageReader::deserialize("INBOUND [FXCM OP]", message.toString());
    }
    std::cout << "[FXCM OP] Logon received" << std::endl;
    std::cout  << "[FXCM OP] Message received " << message.toString() << std::endl;
    _sessioId = sessionID;
    _isConnected = true;
  }


  void OrderHandler::setSessionInf(const FIX::SessionID &sessionId)
  {
    _sessioId = sessionId;
  }
}
}
