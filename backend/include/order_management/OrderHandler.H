#ifndef OrderHandler_H
#define OrderHandler_H

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/fix44/OrderCancelReject.h>
#include <quickfix/SessionID.h>
#include <quickfix/ThreadedSocketInitiator.h>
#include <quickfix/FileStore.h>

namespace Algo { namespace Ib
{
  class OrderHandler : public FIX::Application, public FIX::MessageCracker
  {
    public:
      /// Get singleton instance
      static OrderHandler& getInstance() {
        static OrderHandler instance;
        return instance;
      }
      void start(std::string & fixConfigFile, int enableDebugLog = 0);
      void stop();
      /*!
       * \brief It checks if connection is still alive or not
       */
      bool isConnected();
      void setStartClOrderId(long id);
      OrderHandler();

      void onMessage(const FIX44::Logout &message, const FIX::SessionID &sessionID);
      void onMessage(const FIX44::Logon &message,const FIX::SessionID &sessionID);

    private:
      void onCreate(const FIX::SessionID &sessionID);
      void onLogon(const FIX::SessionID &sessionID);
      void onLogout(const FIX::SessionID &sessionID);

      /**
       * Message from Client to FIXAdapter.
       *
       * @return void
       */
      void toAdmin(FIX::Message&, const FIX::SessionID &sessionId);
      void setHeader(FIX::Message&, const FIX::SessionID &sessionId);
      /**
       * Heartbeat/logon messages received from FIXAdapter.
       *
       * @return void
       */
      void fromAdmin (const FIX::Message & message,
          const FIX::SessionID &) throw (FIX::FieldNotFound,
            FIX::IncorrectDataFormat,
            FIX::IncorrectTagValue,
            FIX::RejectLogon);
      /**
       * Trade message to FIXAdapter from Client.
       *
       * @return void
       */
      void toApp(FIX::Message &message,const FIX::SessionID&) throw (FIX::DoNotSend);
      void fromApp (const FIX::Message & message,
          const FIX::SessionID & sessionID) throw (FIX::FieldNotFound,
            FIX:: IncorrectDataFormat,
            FIX::IncorrectTagValue,
            FIX::UnsupportedMessageType);
      void setSessionInf(const FIX::SessionID &sessionId);

    private:
      FIX::ThreadedSocketInitiator *_initiator;
      FIX::SessionSettings         *_settings;
      FIX::FileStoreFactory        *_storeFactory;
      FIX::ScreenLogFactory        *_logFactory;

      static long                  _clOrdId;
      static FIX::SessionID        _sessioId;
      static int                   _enableDebugLog;
      bool                         _isConnected;
  };
}
}

#endif
