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
  wifiD->init = initGame;
  wifiD->run = runGame;
  scoreBoard->debug = (char *)"Debug";
  smartdisplay_init();
  auto display = lv_display_get_default();
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
  initWifiButtons(X_OFFSET, Y_OFFSET);
  drawWifiButtons(wifiButtons);
  for(int i = 0; i < WIFI_BUTTONS; i++)
  {
    wifiButtons->Buttons[i].callback = wifi_button_click;
  }
  game->gameNumber = 0;
  wifiD->player_symbol = -1;
  wifiD->opponent_symbol = -1;
}

unsigned long lv_last_tick = millis();

void loop()
{
  auto const now = millis();
  if (now > lv_last_tick) {
      lv_tick_inc(now - lv_last_tick);
  } else {
      lv_tick_inc(1); // Handle timer wraparound or other anomalies
  }
  lv_last_tick = now;
  // Update the UI
  lv_timer_handler();
  delay(5);
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