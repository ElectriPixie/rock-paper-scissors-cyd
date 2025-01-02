#ifndef UTIL_H
    #define UTIL_H
    #include <Arduino.h>
    #include <stdlib.h>
    #include <string.h>
    #include <lvgl.h>
    #include <widgets/label/lv_label.h>
    #include <widgets/button/lv_button.h>
    typedef void (*event_cb_t)(lv_event_t *event);
    char* numStr(int number);
    void clearScreen();
#endif /* UTIL_H */
