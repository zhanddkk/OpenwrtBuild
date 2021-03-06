#include "font.h"

#define _FONT_16X16_ZH_DATA_STEP  (32) // (16 * 16 / 8)
static const uint8_t _font_16x16_zh[] = {
    // "  ",
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // "一",
    0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,0x80,0x80,0xC0,0x80,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // "二",
    0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,
    0x08,0x08,0x08,0x0C,0x08,0x00,0x00,0x00,
    0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x10,0x10,0x10,0x10,0x10,0x18,0x10,0x00,

    // "三",
    0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,
    0x84,0x84,0x84,0xC4,0x86,0x04,0x00,0x00,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x30,0x20,0x00,

    // "四",
    0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,
    0x04,0xFC,0x04,0x04,0x04,0xFE,0x04,0x00,
    0x00,0x7F,0x20,0x28,0x24,0x23,0x20,0x20,
    0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,

    // "五",
    0x00,0x04,0x84,0x84,0x84,0x84,0xFC,0x84,
    0x84,0x84,0x84,0xC4,0x86,0x84,0x00,0x00,
    0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,
    0x40,0x40,0x40,0x7F,0x40,0x60,0x40,0x00,

    // "六",
    0x20,0x20,0x20,0x20,0x20,0x22,0x2C,0x38,
    0x20,0x20,0x20,0x20,0x20,0x30,0x20,0x00,
    0x00,0x40,0x20,0x18,0x06,0x03,0x00,0x00,
    0x00,0x01,0x02,0x0C,0x38,0x70,0x00,0x00,

    // "日",
    0x00,0x00,0x00,0xFE,0x82,0x82,0x82,0x82,
    0x82,0x82,0x82,0xFF,0x02,0x00,0x00,0x00,
    0x00,0x00,0x00,0x7F,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x7F,0x00,0x00,0x00,0x00,

    // "周", 
    0x00,0x00,0xFE,0x82,0x92,0x92,0x92,0xFE,
    0x92,0x9A,0xD2,0x82,0xFF,0x02,0x00,0x00,
    0x40,0x20,0x1F,0x00,0x00,0x7E,0x22,0x22,
    0x22,0x3F,0x42,0x80,0x7F,0x00,0x00,0x00,
};

static const uint8_t *_ch_data_16x16_zh(char ch)
{
    int offset = 0;
    const uint8_t *p = _font_16x16_zh;
    int cnt = sizeof(_font_16x16_zh) / _FONT_16X16_ZH_DATA_STEP;
    if ((ch < 0) || (ch >= cnt)) {
        ch = 0;
    }

    offset = ch * _FONT_16X16_ZH_DATA_STEP;

    return p + offset;
}

const struct font font_16x16_zh = {
    16, 16, _ch_data_16x16_zh
};
