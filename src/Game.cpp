#include "Game.h"
Game _game;

Game* getGame() 
{
    return &_game;
}

void addKeyPadCB(KeyPad *keyPad) 
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    lv_obj_add_event_cb(keyPad->Buttons[i].lv_button, keyPad->Buttons[i].callback, LV_EVENT_CLICKED, &keyPad->Buttons[i]);
  }
}

void runGame(Game *game)
{
  ScoreBoard *scoreBoard = getScoreBoard();
  int result = compareSymbol(game->player_symbol, game->opponent_symbol);
  updateScoreBoard(scoreBoard, result);
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

void initGame(Game *game)
{
  KeyPad *keyPad = getKeyPad();
  ScoreBoard *scoreBoard = getScoreBoard();
  game->gameNumber++;
  initKeyPad(keyPad, X_OFFSET, Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT);
  initScoreBoard(scoreBoard);
  drawKeyPad(keyPad);
  drawScoreBoard(scoreBoard);
}

void rps_button_click_cb(lv_event_t *event)
{
  struct Button *button;
  button = (struct Button *)lv_event_get_user_data(event);
  sendSymbol(button->number);
}

void wifi_button_click_cb(lv_event_t *event)
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
  //if(Button->number == CPU_OPPONENT)
}

void sendSymbol(int symbol)
{
  Game *game = getGame();
  ScoreBoard *scoreBoard = getScoreBoard();
  game->player_symbol = symbol;
  char *message = symbolStr(symbol);
  if(DEBUG)
  {
    if(game->Type == SERVER)
    {
      scoreBoard->debug = message;
      drawScoreBoard(scoreBoard);
    }
  }
  if(game->Type == CLIENT)
  {
    sendMessage(message);
    //readServerResponse();
  }
}