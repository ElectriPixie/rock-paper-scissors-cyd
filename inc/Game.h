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
    void initGame(Game *game);
    void runGame(Game *game);
    void sendSymbol(int symbol);

    int compareSymbol(int player_symbol, int opponent_symbol);
    void rps_button_click_cb(lv_event_t *event);
    void wifi_button_click_cb(lv_event_t *event);
#endif /* GAME_H */