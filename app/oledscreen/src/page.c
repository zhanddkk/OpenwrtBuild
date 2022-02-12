#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "page.h"
#include "font.h"
#include "ssd1306.h"

struct time_page_data
{
    // page common data
    int force_update;
    
    // private data
    time_t last_time;
};

static struct time_page_data _time_page_data = {
    1,
    0
};

static int _time_task(void *data)
{
    time_t cur_tm;
    struct time_page_data *pd = (struct time_page_data *)data;
    time(&cur_tm);

    if ((cur_tm != pd->last_time) || pd->force_update) {
        char ch;
        int i;
        int x = 0, y = 0;
        struct tm time_data;
        struct tm last_time_data;

        const struct font *pfont = &font_20x40;

        localtime_r(&cur_tm, &time_data);
        localtime_r(&pd->last_time, &last_time_data);

        // time
        if ((time_data.tm_hour != last_time_data.tm_hour) || pd->force_update) {
            ch = (char)(time_data.tm_hour / 10) + '0';
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(ch), 0);
            x += pfont->w;
            ch = (char)(time_data.tm_hour % 10) + '0';
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(ch), 0);
            x += pfont->w;
        } else {
            x += pfont->w << 1;
        }

        if (pd->force_update) {
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(':'), 0);
        }
        x += pfont->w;

        if ((time_data.tm_min != last_time_data.tm_min) || pd->force_update) {
            ch = (char)(time_data.tm_min / 10) + '0';
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(ch), 0);
            x += pfont->w;
            ch = (char)(time_data.tm_min % 10) + '0';
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(ch), 0);
            x += pfont->w;
        } else {
            x += pfont->w << 1;
        }

        // date
        if ((time_data.tm_mday != last_time_data.tm_mday) || pd->force_update) {
            char buf[16];
            int n = snprintf(buf, sizeof(buf), "%04d-%02d-%02d", time_data.tm_year + 1900, time_data.tm_mon + 1, time_data.tm_mday);
            x = 4;
            y += pfont->h >> 3;
            pfont = &font_12x24;
            for (i = 0; i < n; i++) {
                ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(buf[i]), 0);
                x += pfont->w;
            }
            x = 108;
            y = 0;
            pfont = &font_16x16_zh;
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(8), 0);
            y += pfont->h >> 3;
            buf[0] = time_data.tm_wday == 0? 7 : (uint8_t)time_data.tm_wday;
            ssd1306_draw_bitmap(x, y, pfont->w, pfont->h >> 3, pfont->pf_ch_data(buf[0]), 0);
        }
        pd->last_time = cur_tm;
    } else {
        usleep(500000);
    }
    return 0;
}

static struct page_disp _time_page = {
    "time",
    _time_task,
    (void *)&_time_page_data
};

struct page_disp *get_next_page(void)
{
    return &_time_page;
}
