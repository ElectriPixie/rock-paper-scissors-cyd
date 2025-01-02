#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp32_smartdisplay.h>
#include "Define.h"
#include "Game.h"

ScoreBoard *scoreBoard = getScoreBoard();
KeyPad *keyPad = getKeyPad();
Game *game = getGame();
WifiD *wifiD = getWifiD();
WifiButtons *wifiButtons = getWifiButtons();
ClientState *clients = getClients();

void setup()
{
  wifiD->Enabled = 0;
  scoreBoard->debug = (char *)"Debug";
  smartdisplay_init();
  auto display = lv_display_get_default();
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
  initWifiButtons(X_OFFSET, Y_OFFSET);
  game->gameNumber = 0;
  game->player_symbol = -1;
  game->opponent_symbol = -1;
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
  if(wifiD->Enabled)
  {
    if(wifiD->Type == SERVER)
    {
      acceptNewClients();
      handleClients();
    }
    if(wifiD->Type == CLIENT)
    {
      readServerResponse();
    }
  }
}