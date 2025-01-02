#ifndef BUTTONS_H
    #define BUTTONS_H
    #include "Define.h"
    #include "Util.h"

    typedef void (*ClickFunction)(void *obj);

    typedef struct Button {
        int x, y;       
        int w, h; 
        int number;
        char *text;
        lv_obj_t *lv_button;
        ClickFunction callback;
    } Button;

    void addButton(Button *button, int i, int x, int y, int w, int h, const char *text);
    void addLvButton(Button *button, lv_obj_t *lv_button, int x, int y, int w, int h, const char * label_txt);
    void drawButton(Button *button, int x, int y, int w, int h);
    void button_click_cb(lv_event_t *event);
#endif /* BUTTONS_H */
