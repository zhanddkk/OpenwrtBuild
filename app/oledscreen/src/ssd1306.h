#ifndef SSD1306_H_
#define SSD1306_H_
#include <stdint.h>

#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      8

extern int ssd1306_init(void);
extern void ssd1306_dinit(void);
extern void ssd1306_clear(int mode);
extern void ssd1306_draw_bitmap(uint8_t column, uint8_t row, uint8_t w, uint8_t h, const uint8_t *data, int mode);
#endif
