#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>

#define LOG_INFO(fmt, ...)    printf("INFO :[%4d]" __SOURCE_F__ ":: " fmt "\n", __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   printf("ERROR:[%4d]" __SOURCE_F__ ":: " fmt "\n", __LINE__, ##__VA_ARGS__)
#endif
