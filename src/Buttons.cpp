#include "Buttons.h"
void drawButton(struct Button *button, int x, int y, int w, int h)
{
  addLvButton(button->lv_button, x, y, w, h, button->text);
}

void addButton(struct Button *button, int i, int x, int y, int w, int h, const char *text)
{
  button->x = x;
  button->y = y;
  button->w = w;
  button->h = h;
  button->number = i;
  button->text = (char *)text;
  //button->callback = callback;
}

void addLvButton(lv_obj_t *lv_button, int x, int y, int w, int h, const char * label_txt)
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

    lv_button = lv_button_create(lv_scr_act());
    lv_obj_remove_style_all(lv_button);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(lv_button, &style, 0);
    lv_obj_add_style(lv_button, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(lv_button, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t * label = lv_label_create(lv_button);
    lv_label_set_text(label, label_txt);
    lv_obj_center(label);
    lv_obj_set_x(lv_button, x);
    lv_obj_set_y(lv_button, y);
    lv_obj_set_width(lv_button, w);
    lv_obj_set_height(lv_button, h);
}