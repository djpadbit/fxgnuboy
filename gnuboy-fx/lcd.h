#ifndef __LCD_FX_H__
#define __LCD_FX_H__

extern int lcd_xoff,lcd_yoff;
extern int lcd_scalex,lcd_scaley;
extern uint8_t lcd_show_debug_info,lcd_gray_enabled,lcd_fps_regul;
void lcd_update_scaling();
void lcd_update_gray(uint8_t state);

#endif