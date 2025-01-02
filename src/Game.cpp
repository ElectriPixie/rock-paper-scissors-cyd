#include "Game.h"
Game _game;

Game* getGame() 
{
    return &_game;
}

void runGame()
{
  Game *game = getGame();
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

void initGame()
{
  Game *game = getGame();
  KeyPad *keyPad = getKeyPad();
  ScoreBoard *scoreBoard = getScoreBoard();
  game->gameNumber++;
  initKeyPad(keyPad, X_OFFSET, Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT);
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
  //if(button->number == CPU_OPPONENT)
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
  }
}