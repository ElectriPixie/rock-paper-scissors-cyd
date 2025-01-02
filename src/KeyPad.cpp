#include "KeyPad.h"
KeyPad _keyPad;
WifiButtons _wifiButtons;

KeyPad* getKeyPad() {
    return &_keyPad;
}

WifiButtons* getWifiButtons() {
    return &_wifiButtons;
}

void initKeyPad(struct KeyPad* keyPad, int x_offset, int y_offset, int w, int h)
{
  int x = x_offset;
  int y = y_offset;
  int xw;
  int yh;
  xw = (w - x_offset*4 )/ 3; 
  yh = (h - y_offset*2 ); 
  keyPad->w = w;
  keyPad->h = h;
  keyPad->x_offset = x_offset;
  keyPad->y_offset = y_offset;
  addButton(&(keyPad->Buttons[ROCK]), ROCK, x, y, xw, yh, "rock");
  x = x+xw+keyPad->x_offset;
  addButton(&(keyPad->Buttons[PAPER]), PAPER, x, y, xw, yh, "paper");
  x = x+xw+keyPad->x_offset;
  addButton(&(keyPad->Buttons[SCISSORS]), SCISSORS, x, y, xw, yh, "scissors");
}

void drawKeyPad(struct KeyPad* keyPad)
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    drawButton(&(keyPad->Buttons[i]), keyPad->Buttons[i].x, keyPad->Buttons[i].y, keyPad->Buttons[i].w, keyPad->Buttons[i].h);
  }
}

void initWifiButtons(int x_offset, int y_offset)
{
  WifiButtons *wifiButtons = getWifiButtons();
  int x = x_offset;
  int y = y_offset;
  int w = SCREEN_WIDTH;
  int h = SCREEN_HEIGHT;
  int xw;
  int yh;
  xw = (w - x_offset*3 )/ 3; 
  yh = (h - y_offset*2 ); 
  addButton(&(wifiButtons->Buttons[SERVER]), SERVER, x, y, xw, yh, "Server");
  x = x+xw+x_offset;
  addButton(&(wifiButtons->Buttons[CLIENT]), CLIENT, x, y, xw, yh, "Client");
  x = x+xw+x_offset;
  addButton(&(wifiButtons->Buttons[CPU_OPPONENT]), CPU_OPPONENT, x, y, xw, yh, "CPU");
}

void drawWifiButtons(WifiButtons *wifiButtons)
{
  drawButton(&(wifiButtons->Buttons[SERVER]), wifiButtons->Buttons[SERVER].x, wifiButtons->Buttons[SERVER].y, wifiButtons->Buttons[SERVER].w, wifiButtons->Buttons[SERVER].h);
  drawButton(&(wifiButtons->Buttons[CLIENT]), wifiButtons->Buttons[CLIENT].x, wifiButtons->Buttons[CLIENT].y, wifiButtons->Buttons[CLIENT].w, wifiButtons->Buttons[CLIENT].h);
  drawButton(&(wifiButtons->Buttons[CPU_OPPONENT]), wifiButtons->Buttons[CPU_OPPONENT].x, wifiButtons->Buttons[CPU_OPPONENT].y, wifiButtons->Buttons[CPU_OPPONENT].w, wifiButtons->Buttons[CPU_OPPONENT].h);
}