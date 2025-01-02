#ifndef SERVER_H
  #define SERVER_H
  #include <WiFi.h>
  #include "Define.h"

  typedef struct WifiD{
    int Type;
    WiFiServer* server;
    WiFiClient* client;
    bool Enabled;
  } WifiD;

  typedef struct ClientState {
      WiFiClient client; 
      bool active;
      String buffer;
  } ClientState;

  WifiD *getWifiD();
  ClientState *getClientState();
  void acceptNewClients();
  void readServerResponse();
  void connectToServer();
  void handleClients();
  void sendMessage(char *message);
  void onClientConnect(const WiFiEvent_t event, const WiFiEventInfo_t info);
  void initServer();
#endif /* SERVER_H */