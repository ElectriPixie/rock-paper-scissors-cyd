#ifndef SCOREBOARD_H
  #define SCOREBOARD_H

  // Description: Header file for the scoreboard module.
  #include <Arduino.h>
  #include "Define.h"
  #include "Util.h"
  #include <string.h>
  #include <lvgl.h>
  #include <widgets/label/lv_label.h>
  #include <widgets/button/lv_button.h>

  typedef struct ScoreBoard {
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
  } ScoreBoard;

  ScoreBoard* getScoreBoard();
  char* numStr(int number);
  void initScoreBoard(ScoreBoard* scoreBoard);
  void drawScoreBoard(ScoreBoard* scoreBoard);
  void updateScoreBoard(ScoreBoard* scoreBoard, int result);
  void resetScoreBoard(ScoreBoard* scoreBoard);
#endif /* SCOREBOARD_H */