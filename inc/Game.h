#ifndef GAME_H
    #define GAME_H
    #include "KeyPad.h"
    #include "ScoreBoard.h"
    #include "Server.h"

    void initGame();
    void runGame();
    void sendSymbol(int symbol);
    int compareSymbol(int player_symbol, int opponent_symbol);
    void rps_button_click(void *obj);
    void wifi_button_click(void *obj);
    void addKeyPadCB(KeyPad *keyPad);
    void addWifiButtonsCB(WifiButtons *wifiButtons);
    void showResult(int result, const char *result_msg);
#endif /* GAME_H */