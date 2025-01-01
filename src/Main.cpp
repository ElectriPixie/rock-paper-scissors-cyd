#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp32_smartdisplay.h>
#include <lvgl.h>
#include <widgets/label/lv_label.h>
#include <widgets/button/lv_button.h>
#include <string.h>
#include <WiFi.h>
#include "WifiPasswd.h"

//Set USE_WIFI to 1 to enable wifi currently only AP mode is setup
//USE_WIFI_AP needs to be 1 for the AP to be enabled, edit the settings in inc/WifiPasswd.h before enabling
#define USE_WIFI 0 
#define USE_WIFI_AP 1

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
#define ROCK 0
#define PAPER 1
#define DRAW 0
#define WIN 1
#define LOSE 2
#define SCISSORS 2
#define SB_SHOW_DRAWS 0

typedef void (*event_cb_t)(lv_event_t *event);

struct ScoreBoard{
  int win;
  int lose;
  int draw;
  int win_x;
  int win_y;
  int lose_x;
  int lose_y;
  int draw_x;
  int draw_y;
  lv_obj_t *sb_win; 
  lv_obj_t *sb_lose;
  lv_obj_t *sb_draw;
};

struct Button {
    int x, y;       
    int w, h; 
    int number;
    int color;
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

struct Wifi {
    struct Button Buttons[WIFI_BUTTONS];
};

struct KeyPad KeyPad;
struct Wifi Wifi;
struct ScoreBoard ScoreBoard;

void initGame(struct KeyPad *KeyPad, struct ScoreBoard *ScoreBoard);
void initKeyPad(struct KeyPad* KeyPad, int x_offset, int y_offset, int w, int h, int color);
void addButton(struct KeyPad* KeyPad, int i, int x, int y, int w, int h, int color, const char *text);
void lv_button(struct Button * kp_button, lv_obj_t *button, int x, int y, int w, int h, const char * label_txt);
void drawButton(struct Button *Button, int x, int y, int w, int h);
void drawKeyPad(struct KeyPad* KeyPad);
static void rps_button_click_cb(lv_event_t *event);
void sendSymbol(int symbol);
void initWifi(const char* ssid, const char* password, int channel, int ssid_hidden, int max_connections);
static void wifi_button_click_cb(lv_event_t *event);
void initScoreBoard(struct ScoreBoard* ScoreBoard);
void drawScoreBoard(struct ScoreBoard* ScoreBoard);
void updateScoreBoard(struct ScoreBoard* ScoreBoard, int result);
void resetScoreBoard(struct ScoreBoard* ScoreBoard);
int compareSymbol(int player_symbol, int opponent_symbol);
void game(int player_symbol, int opponent_symbol);


char* numStr(int number);

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
  ScoreBoard->sb_win = lv_label_create(lv_scr_act());
  ScoreBoard->sb_lose = lv_label_create(lv_scr_act());
  if(SB_SHOW_DRAWS)
  {
    ScoreBoard->sb_lose = lv_label_create(lv_scr_act());
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
}

void addButton(struct Button *Button, int i, int x, int y, int w, int h, int color, const char *text, event_cb_t callback)
{
  Button->x = x;
  Button->y = y;
  Button->w = w;
  Button->h = h;
  Button->number = i;
  Button->color = color;
  Button->text = (char *)text;
  Button->callback = callback;
}

void initKeyPad(struct KeyPad* KeyPad, int x_offset, int y_offset, int w, int h, int color)
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
  addButton(&(KeyPad->Buttons[ROCK]), ROCK, x, y, xw, yh, color, "rock", rps_button_click_cb);
  x = x+xw+KeyPad->x_offset;
  addButton(&(KeyPad->Buttons[PAPER]), PAPER, x, y, xw, yh, color, "paper", rps_button_click_cb);
  x = x+xw+KeyPad->x_offset;
  addButton(&(KeyPad->Buttons[SCISSORS]), SCISSORS, x, y, xw, yh, color, "scissors", rps_button_click_cb);
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

void initWifi(const char* ssid, const char* password, int channel, int ssid_hidden, int max_connections)
{
  if(USE_WIFI_AP)
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password, channel, ssid_hidden, max_connections);
  }
}

void sendSymbol(int symbol)
{
  ScoreBoard.win = symbol;
  drawScoreBoard(&ScoreBoard);
}

void game(int player_symbol, int opponent_symbol)
{
  int result = compareSymbol(player_symbol, opponent_symbol);
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

void initGame(struct KeyPad *KeyPad, struct ScoreBoard *ScoreBoard)
{
    drawKeyPad(KeyPad);
    drawScoreBoard(ScoreBoard);
}

void setup()
{
  smartdisplay_init();
  auto display = lv_display_get_default();
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
  initScoreBoard(&ScoreBoard);
  initKeyPad(&KeyPad, 5, 40, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_GREEN);
  if(USE_WIFI)
  {
    initWifi(ssid, password, 11, 0, 4);
  }
  initGame(&KeyPad, &ScoreBoard);
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
}