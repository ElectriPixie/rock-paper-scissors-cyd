#include "Server.h"
struct WifiD _wifiD;

WifiD *getWifiD() {
    return &_wifiD;
}

struct ClientState _clients[MAX_CLIENTS];

ClientState *getClients() {
    return &_clients;
}

void sendMessage(char *message) {
    if (wifiD->client->connected()) {
        wifiD->client->println(message);
    }
}

void handleClients() {
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
                      game->opponent_symbol = ROCK;
                    }
                    if(clients[i].buffer == "PAPER")
                    {
                      game->opponent_symbol = PAPER;
                    }
                    if(clients[i].buffer == "SCISSORS")
                    {
                      game->opponent_symbol = SCISSORS;
                    }
                    char *message = symbolStr(game->player_symbol);
                    client.println(message);
                    runGame(game);
                    // Clear the buffer for the next request
                    clients[i].buffer = "";
                }
            }
        }
    }
}

void connectToServer() {
    wifiD->client = new WiFiClient();
    if (!wifiD->client->connected()) {
        if (wifiD->client->connect(WiFi.gatewayIP(), SERVER_PORT)) {
          clearScreen();
          initGame(game);
          wifiD->Enabled = 1;
        }
    }
}

void readServerResponse() {
  if (wifiD->client->connected() && wifiD->client->available()) {
      String response = wifiD->client->readStringUntil('\n');
      response.trim();
      if(response == "ROCK")
      {
        game->opponent_symbol = ROCK;
      }
      if(response == "PAPER")
      {
        game->opponent_symbol = PAPER;
      }
      if(response == "SCISSORS")
      {
        game->opponent_symbol = SCISSORS;
      }
      if(DEBUG)
      {
        scoreBoard->debug = symbolStr(game->opponent_symbol);
        drawScoreBoard(scoreBoard);
      }
      runGame(game);
  }
}

void acceptNewClients() {
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
  initGame(game);
  wifiD->Enabled = 1;
}

void initServer()
{
  game->Type = SERVER;
  wifiD->Type = SERVER;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, channel, ssid_hidden, max_connection);
  WiFi.onEvent(onClientConnect, ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
  wifiD->server = new WiFiServer(SERVER_PORT);
  wifiD->server->begin();
}

void initClient()
{
    game->Type = CLIENT;
    wifiD->Type = CLIENT;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }
    connectToServer();
}