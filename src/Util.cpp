#include "Util.h"

char* numStr(int number) {
    std::string strNumber = std::to_string(number);
    char* ret = new char[strNumber.length() + 1];
    strcpy(ret, strNumber.c_str());
   return ret;
}

void clearScreen()
{
  lv_obj_t *current_screen = lv_scr_act(); // Get the current active screen
  lv_obj_clean(current_screen);           // Remove all objects on the screen
}