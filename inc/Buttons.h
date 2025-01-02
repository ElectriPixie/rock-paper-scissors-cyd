#ifndef BUTTONS_H
    #define BUTTONS_H
    #include "Define.h"
    #include "Util.h"

    void sendSymbol(char symbol); 

    typedef void (*event_cb_t)(lv_event_t *event);

    typedef struct Button {
        int x, y;       
        int w, h; 
        int number;
        char *text;
        lv_obj_t *lv_button;
        event_cb_t callback;
    } Button;

    void addButton(Button *button, int i, int x, int y, int w, int h, const char *text);
    void addLvButton(lv_obj_t *lv_button, int x, int y, int w, int h, const char * label_txt);
    void drawButton(Button *button, int x, int y, int w, int h);
#endif /* BUTTONS_H */
