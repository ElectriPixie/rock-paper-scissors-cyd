#ifndef GAME_H
    #define GAME_H
    #include "KeyPad.h"
    #include "ScoreBoard.h"
    #include "Server.h"

    typedef struct Game {
        int gameNumber;
        int player_symbol;
        int opponent_symbol;
        int state;
        int Type;
    } Game;

    Game* getGame();
    void initGame();
    void runGame();
    void sendSymbol(int symbol);
    int compareSymbol(int player_symbol, int opponent_symbol);
    void rps_button_click(void *obj);
    void wifi_button_click(void *obj);
    void addKeyPadCB(KeyPad *keyPad);
    void addWifiButtonsCB(WifiButtons *wifiButtons);
#endif /* GAME_H */