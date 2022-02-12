#ifndef PAGE_H_
#define PAGE_H_

#include <stdint.h>

typedef int (*t_page_task_pf)(void *data);


struct page_disp
{
    const char *title;
    t_page_task_pf task;
    void *data;
};

extern struct page_disp *get_next_page(void);
#endif
