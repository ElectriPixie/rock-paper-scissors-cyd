#ifndef SERVER_H
  #define SERVER_H
  #include <WiFi.h>
  #include <ctime>
  #include "Define.h"
  #include "WifiPasswd.h"
  #include "Util.h"
  #include "ScoreBoard.h"

  typedef void (*GameFunction)();

  typedef struct WifiD{
    int Type;
    int player_symbol;
    int opponent_symbol;
    WiFiServer* server;
    WiFiClient* client;
    bool Enabled;
    GameFunction init;
    GameFunction run;
  } WifiD;

  typedef struct ClientState {
      WiFiClient client; 
      bool active;
      String buffer;
  } ClientState;

  WifiD *getWifiD();
  ClientState *getClients();
  void acceptNewClients();
  void readServerResponse();
  void connectToServer();
  void handleClients();
  void sendMessage(char *message);
  void onClientConnect(const WiFiEvent_t event, const WiFiEventInfo_t info);
  void initServer();
  void initClient();
  void sendSymbol(int symbol);
  char* symbolStr(int symbol);
  void initCpuOpponent();
  void sendCpuMessage(int symbol);
#endif /* SERVER_H */