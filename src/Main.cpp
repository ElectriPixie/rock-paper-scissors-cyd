#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp32_smartdisplay.h>
#include <lvgl.h>
#include <widgets/label/lv_label.h>
#include <widgets/button/lv_button.h>
#include <string.h>
#include <WiFi.h>
#include "WifiPasswd.h"

#define DEBUG 1
#define SERVER_PORT 80

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_BUTTONS 3
#define WIFI_BUTTONS 2
#define WIN_X 5
#define WIN_Y 5
#define LOSE_X 105
#define LOSE_Y 5
#define DRAW_X 210
#define DRAW_Y 5
#define DEBUG_X 5
#define DEBUG_Y 215
#define ROCK 0
#define PAPER 1
#define SCISSORS 2
#define DRAW 0
#define WIN 1
#define LOSE 2
#define SB_SHOW_DRAWS 1
#define SERVER 0
#define CLIENT 1
#define X_OFFSET 5
#define Y_OFFSET 40
#define MAX_CLIENTS 1

typedef void (*event_cb_t)(lv_event_t *event);

struct ClientState {
    WiFiClient client; 
    bool active;
    String buffer;
};

struct ScoreBoard{
  int win;
  int lose;
  int draw;
  char *debug;
  int win_x;
  int win_y;
  int lose_x;
  int lose_y;
  int draw_x;
  int draw_y;
  int debug_x;
  int debug_y;
  lv_obj_t *sb_win; 
  lv_obj_t *sb_lose;
  lv_obj_t *sb_draw;
  lv_obj_t *sb_debug;
};

struct Button {
    int x, y;       
    int w, h; 
    int number;
    char *text;
    lv_obj_t *button;
    event_cb_t callback;
};

struct KeyPad {
  int x, y;
  int w, h;
  int x_offset;
  int y_offset;
  struct Button Buttons[NUM_BUTTONS];
};

struct WifiD{
  int Type;
  WiFiServer* server; // Pointer to the server
  WiFiClient* client;
  bool Enabled;
};

struct WifiButtons {
    struct Button Buttons[WIFI_BUTTONS];
};

struct Game {
  int gameNumber;
  int player_symbol;
  int opponent_symbol;
  int state;
};

struct KeyPad KeyPad;
struct WifiButtons WifiButtons;
struct ScoreBoard ScoreBoard;
struct WifiD WifiD;
struct Game Game;
ClientState clients[MAX_CLIENTS];

void initGame();
void initKeyPad(struct KeyPad* KeyPad, int x_offset, int y_offset, int w, int h, int color);
void addButton(struct KeyPad* KeyPad, int i, int x, int y, int w, int h, int color, const char *text);
void lv_button(struct Button * kp_button, lv_obj_t *button, int x, int y, int w, int h, const char * label_txt);
void drawButton(struct Button *Button, int x, int y, int w, int h);
void drawKeyPad(struct KeyPad* KeyPad);
static void rps_button_click_cb(lv_event_t *event);
void sendSymbol(int symbol);
void initWifi(const char* ssid, const char* password, int channel, int ssid_hidden, int max_connections);
void initWifiButtons(int x_offset, int y_offset);
static void wifi_button_click_cb(lv_event_t *event);
void initScoreBoard(struct ScoreBoard* ScoreBoard);
void drawScoreBoard(struct ScoreBoard* ScoreBoard);
void updateScoreBoard(struct ScoreBoard* ScoreBoard, int result);
void resetScoreBoard(struct ScoreBoard* ScoreBoard);
int compareSymbol(int player_symbol, int opponent_symbol);
void game();
void initServer();
void initClient();
void clearScreen();
void onClientConnect(const WiFiEvent_t event, const WiFiEventInfo_t info); 
char* numStr(int number);
void sendMessage(char *message);
void readServerResponse();
void connectToServer();


char* numStr(int number) {
    std::string strNumber = std::to_string(number);
    char* ret = new char[strNumber.length() + 1];
    strcpy(ret, strNumber.c_str());
   return ret;
}

void initScoreBoard(struct ScoreBoard* ScoreBoard)
{
  ScoreBoard->win = 0;
  ScoreBoard->lose = 0;
  ScoreBoard->draw = 0;
  ScoreBoard->win_x = WIN_X;
  ScoreBoard->win_y = WIN_Y;
  ScoreBoard->lose_x = LOSE_X;
  ScoreBoard->lose_y = LOSE_Y;
  ScoreBoard->draw_x = DRAW_X;
  ScoreBoard->draw_y = DRAW_Y;
  ScoreBoard->debug_x = DEBUG_X;
  ScoreBoard->debug_y = DEBUG_Y;
  ScoreBoard->sb_win = lv_label_create(lv_scr_act());
  ScoreBoard->sb_lose = lv_label_create(lv_scr_act());
  if(SB_SHOW_DRAWS)
  {
    ScoreBoard->sb_draw = lv_label_create(lv_scr_act());
  }
  if(DEBUG)
  {
    ScoreBoard->sb_debug = lv_label_create(lv_scr_act());
  }
}

void drawScoreBoard(struct ScoreBoard* ScoreBoard)
{
  std::string win_text = "wins: ";
  char *win = numStr(ScoreBoard->win);
  win_text += win;
  std::string lose_text = "losses: ";
  char *lose = numStr(ScoreBoard->lose);
  lose_text += lose;
  lv_label_set_text(ScoreBoard->sb_win, (const char *)win_text.c_str());
  lv_label_set_text(ScoreBoard->sb_lose, (const char *)lose_text.c_str());
  lv_obj_set_x(ScoreBoard->sb_win, ScoreBoard->win_x);
  lv_obj_set_y(ScoreBoard->sb_win, ScoreBoard->win_y);
  lv_obj_set_x(ScoreBoard->sb_lose, ScoreBoard->lose_x);
  lv_obj_set_y(ScoreBoard->sb_lose, ScoreBoard->lose_y);
  if(SB_SHOW_DRAWS)
  {
    std::string draw_text = "draws: ";
    char *draw = numStr(ScoreBoard->draw);
    draw_text += draw;
    lv_label_set_text(ScoreBoard->sb_draw, (const char *)draw_text.c_str());
    lv_obj_set_x(ScoreBoard->sb_draw, ScoreBoard->draw_x);
    lv_obj_set_y(ScoreBoard->sb_draw, ScoreBoard->draw_y);
  }
  if(DEBUG)
  {
    std::string debug_text = "debug: ";
    debug_text += ScoreBoard->debug;
    lv_label_set_text(ScoreBoard->sb_debug, (const char *)debug_text.c_str());
    lv_obj_set_x(ScoreBoard->sb_debug, ScoreBoard->debug_x);
    lv_obj_set_y(ScoreBoard->sb_debug, ScoreBoard->debug_y);
  }
}

void updateScoreBoard(struct ScoreBoard* ScoreBoard, int result)
{
  if(result == WIN)
  {
    ScoreBoard->win++;
  }
  if(result == LOSE)
  {
    ScoreBoard->lose++;
  }
  if(result == DRAW)
  {
    ScoreBoard->draw++;
  }
  drawScoreBoard(ScoreBoard);
}

void resetScoreBoard(struct ScoreBoard* ScoreBoard)
{
  ScoreBoard->win = 0;
  ScoreBoard->lose = 0;
  ScoreBoard->draw = 0;
  drawScoreBoard(ScoreBoard);
}

void drawButton(struct Button *Button, int x, int y, int w, int h)
{
  lv_button(Button, Button->button, x, y, w, h, Button->text);
}

void drawKeyPad(struct KeyPad* KeyPad)
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    drawButton(&(KeyPad->Buttons[i]), KeyPad->Buttons[i].x, KeyPad->Buttons[i].y, KeyPad->Buttons[i].w, KeyPad->Buttons[i].h);
  }
}

static void rps_button_click_cb(lv_event_t *event)
{
  struct Button *Button;
  Button = (struct Button *)lv_event_get_user_data(event);
  sendSymbol(Button->number);
}

static void wifi_button_click_cb(lv_event_t *event)
{
  struct Button *Button;
  Button = (struct Button *)lv_event_get_user_data(event);
  if(Button->number == SERVER)
  {
    initServer();
  }
  if(Button->number == CLIENT)
  {
    initClient();
  }
}

void addButton(struct Button *Button, int i, int x, int y, int w, int h, const char *text, event_cb_t callback)
{
  Button->x = x;
  Button->y = y;
  Button->w = w;
  Button->h = h;
  Button->number = i;
  Button->text = (char *)text;
  Button->callback = callback;
}

void initKeyPad(struct KeyPad* KeyPad, int x_offset, int y_offset, int w, int h)
{
  int x = x_offset;
  int y = y_offset;
  int xw;
  int yh;
  xw = (w - x_offset*4 )/ 3; 
  yh = (h - y_offset*2 ); 
  KeyPad->w = w;
  KeyPad->h = h;
  KeyPad->x_offset = x_offset;
  KeyPad->y_offset = y_offset;
  addButton(&(KeyPad->Buttons[ROCK]), ROCK, x, y, xw, yh, "rock", rps_button_click_cb);
  x = x+xw+KeyPad->x_offset;
  addButton(&(KeyPad->Buttons[PAPER]), PAPER, x, y, xw, yh, "paper", rps_button_click_cb);
  x = x+xw+KeyPad->x_offset;
  addButton(&(KeyPad->Buttons[SCISSORS]), SCISSORS, x, y, xw, yh, "scissors", rps_button_click_cb);
}

void lv_button(Button *kp_button, lv_obj_t *button, int x, int y, int w, int h, const char * label_txt)
{
    /*Init the style for the default state*/
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 3);

    lv_style_set_bg_opa(&style, LV_OPA_100);
    lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_border_opa(&style, LV_OPA_40);
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_shadow_width(&style, 8);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_offset_y(&style, 8);

    lv_style_set_outline_opa(&style, LV_OPA_COVER);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_pad_all(&style, 10);

    /*Init the pressed style*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);

    lv_style_set_translate_y(&style_pr, 5);
    lv_style_set_shadow_offset_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_BLUE, 2));

    button = lv_button_create(lv_scr_act());
    lv_obj_remove_style_all(button);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(button, &style, 0);
    lv_obj_add_style(button, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(button, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t * label = lv_label_create(button);
    lv_label_set_text(label, label_txt);
    lv_obj_center(label);
    lv_obj_set_x(button, x);
    lv_obj_set_y(button, y);
    lv_obj_set_width(button, w);
    lv_obj_set_height(button, h);
    lv_obj_add_event_cb(button, kp_button->callback, LV_EVENT_CLICKED, kp_button);
}

void initWifiButtons(int x_offset, int y_offset)
{
  int x = x_offset;
  int y = y_offset;
  int w = SCREEN_WIDTH;
  int h = SCREEN_HEIGHT;
  int xw;
  int yh;
  xw = (w - x_offset*3 )/ 2; 
  yh = (h - y_offset*2 ); 
  addButton(&(WifiButtons.Buttons[SERVER]), SERVER, x, y, xw, yh, "Server", wifi_button_click_cb);
  x = x+xw+x_offset;
  addButton(&(WifiButtons.Buttons[CLIENT]), CLIENT, x, y, xw, yh, "Client", wifi_button_click_cb);
  drawButton(&(WifiButtons.Buttons[SERVER]), WifiButtons.Buttons[SERVER].x, WifiButtons.Buttons[SERVER].y, WifiButtons.Buttons[SERVER].w, WifiButtons.Buttons[SERVER].h);
  drawButton(&(WifiButtons.Buttons[CLIENT]), WifiButtons.Buttons[CLIENT].x, WifiButtons.Buttons[CLIENT].y, WifiButtons.Buttons[CLIENT].w, WifiButtons.Buttons[CLIENT].h);
}

void initServer()
{
  WifiD.Type = SERVER;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, channel, ssid_hidden, max_connection);
  WiFi.onEvent(onClientConnect, ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
  WifiD.server = new WiFiServer(SERVER_PORT);
  WifiD.server->begin();
}

void onClientConnect(const WiFiEvent_t event, const WiFiEventInfo_t info) 
{
  clearScreen();
  initGame();
  WifiD.Enabled = 1;
}

void initClient()
{
    WifiD.Type = CLIENT;
    WiFi.begin(ssid, password);
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

void sendSymbol(int symbol)
{
  Game.player_symbol = symbol;
  char *message = symbolStr(symbol);
  if(DEBUG)
  {
    if(WifiD.Type == SERVER)
    {
      ScoreBoard.debug = message;
      drawScoreBoard(&ScoreBoard);
    }
  }
  if(WifiD.Type == CLIENT)
  {
    sendMessage(message);
    //readServerResponse();
  }
}

void clearScreen()
{
  lv_obj_t *current_screen = lv_scr_act(); // Get the current active screen
  lv_obj_clean(current_screen);           // Remove all objects on the screen
}

void game()
{
  int result = compareSymbol(Game.player_symbol, Game.opponent_symbol);
  updateScoreBoard(&ScoreBoard, result);
}

int compareSymbol(int player_symbol, int opponent_symbol)
{
  if(player_symbol == ROCK)
  {
    if(opponent_symbol == ROCK)
    {
      return DRAW;
    }
    if(opponent_symbol == PAPER)
    {
      return LOSE;
    }
    if(opponent_symbol == SCISSORS)
    {
      return WIN;
    }
  }
  if(player_symbol == PAPER)
  {
    if(opponent_symbol == ROCK)
    {
      return WIN;
    }
    if(opponent_symbol == PAPER)
    {
      return DRAW;
    }
    if(opponent_symbol == SCISSORS)
    {
      return LOSE;
    }
  }
  if(player_symbol == SCISSORS)
  {
    if(opponent_symbol == ROCK)
    {
      return LOSE;
    }
    if(opponent_symbol == PAPER)
    {
      return WIN;
    }
    if(opponent_symbol == SCISSORS)
    {
      return DRAW;
    }
  }
  return -1;
}

void initGame()
{
    Game.gameNumber++;
    initKeyPad(&KeyPad, X_OFFSET, Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT);
    initScoreBoard(&ScoreBoard);
    drawKeyPad(&KeyPad);
    drawScoreBoard(&ScoreBoard);
}

void setup()
{
  WifiD.Enabled = 0;
  ScoreBoard.debug = (char *)"Debug";
  smartdisplay_init();
  auto display = lv_display_get_default();
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
  initWifiButtons(X_OFFSET, Y_OFFSET);
  Game.gameNumber = 0;
  Game.player_symbol = -1;
  Game.opponent_symbol = -1;
}

void acceptNewClients() {
    WiFiClient newClient = WifiD.server->available();
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
                      Game.opponent_symbol = ROCK;
                    }
                    if(clients[i].buffer == "PAPER")
                    {
                      Game.opponent_symbol = PAPER;
                    }
                    if(clients[i].buffer == "SCISSORS")
                    {
                      Game.opponent_symbol = SCISSORS;
                    }
                    char *message = symbolStr(Game.player_symbol);
                    client.println(message);
                    game();
                    // Clear the buffer for the next request
                    clients[i].buffer = "";
                }
            }
        }
    }
}

void connectToServer() {
    WifiD.client = new WiFiClient();
    if (!WifiD.client->connected()) {
        if (WifiD.client->connect(WiFi.gatewayIP(), SERVER_PORT)) {
          clearScreen();
          initGame();
          WifiD.Enabled = 1;
        }
    }
}

void sendMessage(char *message) {
    if (WifiD.client->connected()) {
        WifiD.client->println(message);
    }
}

void readServerResponse() {
  if (WifiD.client->connected() && WifiD.client->available()) {
      String response = WifiD.client->readStringUntil('\n');
      response.trim();
      if(response == "ROCK")
      {
        Game.opponent_symbol = ROCK;
      }
      if(response == "PAPER")
      {
        Game.opponent_symbol = PAPER;
      }
      if(response == "SCISSORS")
      {
        Game.opponent_symbol = SCISSORS;
      }
      if(DEBUG)
      {
        ScoreBoard.debug = symbolStr(Game.opponent_symbol);
        drawScoreBoard(&ScoreBoard);
      }
      game();
  }
}


auto lv_last_tick = millis();

void loop()
{
  auto const now = millis();
  // Update the ticker
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;
  // Update the UI
  lv_timer_handler();
  if(WifiD.Enabled)
  {
    if(WifiD.Type == SERVER)
    {
      acceptNewClients();
      handleClients();
    }
    if(WifiD.Type == CLIENT)
    {
      readServerResponse();
    }
  }
}