#ifndef SEL_PPSE_H
# define SEL_PPSE_H

#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <unistd.h>

typedef struct t_info
{
    unsigned char   *rid;
    unsigned char   *pix;
    unsigned short  prior;
    struct t_info   *next;
} app_info;



int             un_strncmp(const char *str1, unsigned const char *str2, size_t n);
// void            sort_app_prior(app_info *apps, unsigned short app_num);
// void            print_supported(app_info *apps, unsigned short app_num);
// void            fill_app_info(unsigned char *pRxBuf, unsigned short i, app_info *apps, unsigned short app_num);
// void            init_apps_prior(app_info *apps, unsigned short app_num);
// void            free_all(app_info *apps, unsigned short app_num);
size_t          un_strlen(const unsigned char *str);
size_t          un_strlcpy(unsigned char *dst, const unsigned char *src, size_t size);
unsigned char   *str_to_ustr(const char *str);
// unsigned short  app_mem(unsigned char *pRxBuf, unsigned short i, unsigned short end);
unsigned short  char_to_num(unsigned char first, unsigned char second);
int	un_strcmp(const char *str1, const unsigned char *str2);



#endif