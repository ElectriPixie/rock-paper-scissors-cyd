#include "Server.h"
struct WifiD _wifiD;

WifiD *getWifiD() {
    return &_wifiD;
}

struct ClientState _clients[MAX_CLIENTS];

ClientState *getClients() {
    return _clients;
}

void sendMessage(char *message) {
    WifiD *wifiD = getWifiD();
    if (wifiD->client->connected()) {
        wifiD->client->println(message);
    }
}

void handleClients() {
    ClientState *clients = getClients();
    WifiD *wifiD = getWifiD();
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            WiFiClient& client = clients[i].client;

            if (!client.connected()) {
                clients[i].active = false;
                client.stop();
                continue;
            }

            // Read incoming data
            while (client.available()) {
                char c = client.read();
                clients[i].buffer += c;

                // Process complete requests (e.g., ends with newline)
                if (c == '\n') {
                    clients[i].buffer.trim();
                    if(clients[i].buffer == "ROCK")
                    {
                      wifiD->opponent_symbol = ROCK;
                    }
                    if(clients[i].buffer == "PAPER")
                    {
                      wifiD->opponent_symbol = PAPER;
                    }
                    if(clients[i].buffer == "SCISSORS")
                    {
                      wifiD->opponent_symbol = SCISSORS;
                    }
                    char *message = symbolStr(wifiD->player_symbol);
                    client.println(message);
                    wifiD->run();
                    // Clear the buffer for the next request
                    clients[i].buffer = "";
                }
            }
        }
    }
}

void connectToServer() {
    WifiD *wifiD = getWifiD();
    wifiD->client = new WiFiClient();
    if (!wifiD->client->connected()) {
        if (wifiD->client->connect(WiFi.gatewayIP(), SERVER_PORT)) {
          clearScreen();
          wifiD->init();
          wifiD->Enabled = 1;
        }
    }
}

void readServerResponse() {
  WifiD *wifiD = getWifiD();
  ScoreBoard *scoreBoard = getScoreBoard();
  if (wifiD->client->connected() && wifiD->client->available()) {
      String response = wifiD->client->readStringUntil('\n');
      response.trim();
      if(response == "ROCK")
      {
        wifiD->opponent_symbol = ROCK;
      }
      if(response == "PAPER")
      {
        wifiD->opponent_symbol = PAPER;
      }
      if(response == "SCISSORS")
      {
        wifiD->opponent_symbol = SCISSORS;
      }
      if(DEBUG)
      {
        scoreBoard->debug = symbolStr(wifiD->opponent_symbol);
        drawScoreBoard(scoreBoard);
      }
      wifiD->run();
  }
}

void acceptNewClients() {
    WifiD *wifiD = getWifiD();
    ClientState *clients = getClients();
    WiFiClient newClient = wifiD->server->available();
    if (newClient) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) {
                clients[i].client = newClient;
                clients[i].active = true;
                clients[i].buffer = "";
                return;
            }
        }
        newClient.stop();
    }
}

void onClientConnect(const WiFiEvent_t event, const WiFiEventInfo_t info) 
{
  clearScreen();
  WifiD *wifiD = getWifiD();
  wifiD->init();
  wifiD->Enabled = 1;
}

void initServer()
{
  WifiD *wifiD = getWifiD();
  wifiD->Type = SERVER;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD, CHANNEL, SSID_HIDDEN, MAX_CONNECTION);
  WiFi.onEvent(onClientConnect, ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
  wifiD->server = new WiFiServer(SERVER_PORT);
  wifiD->server->begin();
}

void initClient()
{
    WifiD *wifiD = getWifiD();
    wifiD->Type = CLIENT;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }
    connectToServer();
}

char* symbolStr(int symbol)
{
  char *message;
  if(symbol == ROCK)
  {
    message = (char *)"ROCK";
  }
  if(symbol == PAPER)
  {
    message = (char *)"PAPER";
  }
  if(symbol == SCISSORS)
  {
    message = (char *)"SCISSORS";
  }
  return message;
}