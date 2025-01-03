#include "Game.h"

void runGame()
{
  WifiD *wifiD = getWifiD();
  ScoreBoard *scoreBoard = getScoreBoard();
  KeyPad *keyPad = getKeyPad();
  int result = compareSymbol(wifiD->player_symbol, wifiD->opponent_symbol);
  char *result_msg;
  if(result == WIN)
  {
    result_msg = (char *)"WIN";
  }
  if(result == LOSE)
  {
    result_msg = (char *)"LOOSE";
  }
  if(result == DRAW)
  {
    result_msg = (char *)"DRAW";
  }
  //showResult(result, result_msg);
  //showKeyPad(keyPad);
  updateScoreBoard(scoreBoard, result);
  wifiD->picked = 0;
  wifiD->thrown = 0;
  wifiD->gameNumber++;
}

void showResult(int result, const char *result_msg)
{
  clearScreen();
  lv_obj_t * result_label = lv_label_create(lv_scr_act());
  lv_label_set_text(result_label, result_msg);
  lv_obj_center(result_label);
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
  WifiD *wifiD = getWifiD();
  KeyPad *keyPad = getKeyPad();
  ScoreBoard *scoreBoard = getScoreBoard();
  wifiD->gameNumber++;
  initKeyPad(keyPad, X_OFFSET, Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT);
  for(int i=0; i<NUM_BUTTONS; i++)
  {
    keyPad->Buttons[i].callback = rps_button_click;
  }
  initScoreBoard(scoreBoard);
  drawKeyPad(keyPad);
  drawScoreBoard(scoreBoard);
}

void rps_button_click(void *obj)
{
  Button *button = (Button *)obj;
  sendSymbol(button->number);
}

void wifi_button_click(void *obj)
{
  Button *button = (Button *)obj;
  if(button->number == SERVER)
  {
    initServer();
  }
  if(button->number == CLIENT)
  {
    initClient();
  }
  if(button->number == CPU_OPPONENT)
  {
    initCpuOpponent();
  }
}

void sendSymbol(int symbol)
{
  WifiD *wifiD = getWifiD();
  ScoreBoard *scoreBoard = getScoreBoard();
  KeyPad *keyPad = getKeyPad();
  wifiD->player_symbol = symbol;
  char *message = symbolStr(symbol);
  if(DEBUG)
  {
    if(wifiD->Type == SERVER)
    {
      scoreBoard->debug = message;
      drawScoreBoard(scoreBoard);
    }
  }
  wifiD->picked = 1;
  if(wifiD->Type == CLIENT)
  {
    sendMessage(message);
//    hideKeyPad(keyPad);
//    lv_obj_t * thrown_label = lv_label_create(lv_scr_act());
//    lv_label_set_text(thrown_label, "Thrown...");
//    lv_obj_center(thrown_label);
  }
  if(wifiD->Type == CPU_OPPONENT)
  {
    sendCpuMessage(symbol);
  }
}