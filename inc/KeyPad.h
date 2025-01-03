#ifndef KEYPAD_H
  #define KEYPAD_H
  #include "Buttons.h"
  #include "Util.h"

  typedef struct KeyPad {
      int x, y;
      int w, h;
      int x_offset;
      int y_offset;
      Button Buttons[NUM_BUTTONS];
  } KeyPad;

  typedef struct WifiButtons {
    struct Button Buttons[WIFI_BUTTONS];
  } WifiButtons;

  KeyPad* getKeyPad();
  WifiButtons* getWifiButtons();
  void initKeyPad(KeyPad *keyPad, int x_offset, int y_offset, int w, int h);
  void drawKeyPad(KeyPad *keyPad);
  void initWifiButtons(int x_offset, int y_offset);
  void drawWifiButtons(WifiButtons *wifiButtons);
  void hideKeyPad(KeyPad *keyPad);
  void showKeyPad(KeyPad *keyPad);
#endif /* KEYPAD_H */