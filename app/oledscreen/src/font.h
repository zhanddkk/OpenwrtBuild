#ifndef FONT_H_
#define FONT_H_
#include <stdint.h>

typedef const uint8_t * (*t_font_ch_data) (char ch);

struct font
{
    int w;
    int h;
    t_font_ch_data pf_ch_data;
};

extern const struct font font_6x8;
extern const struct font font_12x24;
extern const struct font font_20x40;
extern const struct font font_16x16_zh;
#endif
