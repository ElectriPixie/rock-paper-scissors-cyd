#include "ScoreBoard.h"

ScoreBoard _scoreBoard;

ScoreBoard* getScoreBoard() {
    return &_scoreBoard;
}

void initScoreBoard(ScoreBoard* scoreBoard)
{
  scoreBoard->win = 0;
  scoreBoard->lose = 0;
  scoreBoard->draw = 0;
  scoreBoard->win_x = WIN_X;
  scoreBoard->win_y = WIN_Y;
  scoreBoard->lose_x = LOSE_X;
  scoreBoard->lose_y = LOSE_Y;
  scoreBoard->draw_x = DRAW_X;
  scoreBoard->draw_y = DRAW_Y;
  scoreBoard->debug_x = DEBUG_X;
  scoreBoard->debug_y = DEBUG_Y;
  scoreBoard->sb_win = lv_label_create(lv_scr_act());
  scoreBoard->sb_lose = lv_label_create(lv_scr_act());
  if(SB_SHOW_DRAWS)
  {
    scoreBoard->sb_draw = lv_label_create(lv_scr_act());
  }
  if(DEBUG)
  {
    scoreBoard->sb_debug = lv_label_create(lv_scr_act());
  }
}

void drawScoreBoard(ScoreBoard* scoreBoard)
{
  std::string win_text = "wins: ";
  char *win = numStr(scoreBoard->win);
  win_text += win;
  std::string lose_text = "losses: ";
  char *lose = numStr(scoreBoard->lose);
  lose_text += lose;
  lv_label_set_text(scoreBoard->sb_win, (const char *)win_text.c_str());
  lv_label_set_text(scoreBoard->sb_lose, (const char *)lose_text.c_str());
  lv_obj_set_x(scoreBoard->sb_win, scoreBoard->win_x);
  lv_obj_set_y(scoreBoard->sb_win, scoreBoard->win_y);
  lv_obj_set_x(scoreBoard->sb_lose, scoreBoard->lose_x);
  lv_obj_set_y(scoreBoard->sb_lose, scoreBoard->lose_y);
  if(SB_SHOW_DRAWS)
  {
    std::string draw_text = "draws: ";
    char *draw = numStr(scoreBoard->draw);
    draw_text += draw;
    lv_label_set_text(scoreBoard->sb_draw, (const char *)draw_text.c_str());
    lv_obj_set_x(scoreBoard->sb_draw, scoreBoard->draw_x);
    lv_obj_set_y(scoreBoard->sb_draw, scoreBoard->draw_y);
  }
  if(DEBUG)
  {
    std::string debug_text = "debug: ";
    debug_text += scoreBoard->debug;
    lv_label_set_text(scoreBoard->sb_debug, (const char *)debug_text.c_str());
    lv_obj_set_x(scoreBoard->sb_debug, scoreBoard->debug_x);
    lv_obj_set_y(scoreBoard->sb_debug, scoreBoard->debug_y);
  }
}

void updateScoreBoard(ScoreBoard* scoreBoard, int result)
{
  if(result == WIN)
  {
    scoreBoard->win++;
  }
  if(result == LOSE)
  {
    scoreBoard->lose++;
  }
  if(result == DRAW)
  {
    scoreBoard->draw++;
  }
  drawScoreBoard(scoreBoard);
}

void resetScoreBoard(ScoreBoard* scoreBoard)
{
  scoreBoard->win = 0;
  scoreBoard->lose = 0;
  scoreBoard->draw = 0;
  drawScoreBoard(scoreBoard);
}