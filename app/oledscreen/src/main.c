#include <stdio.h>

#include "log.h"
#include "font.h"
#include "ssd1306.h"
#include "page.h"

int main(int argc, const char *argv[])
{
    struct page_disp *last_pd = NULL;

    ssd1306_init();
    ssd1306_clear(0);

    while (1)
    {
        struct page_disp *pd = get_next_page();
        if (pd) {
            if (last_pd != pd) {
                ((int *)(pd->data))[0] = 1;
            } else {
                ((int *)(pd->data))[0] = 0;
            }
            pd->task(pd->data);
            last_pd = pd;
        }
    }

    return 0;
}
