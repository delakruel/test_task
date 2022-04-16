#ifndef SEL_PPSE_H
# define SEL_PPSE_H

#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <unistd.h>

typedef struct s_info
{
    unsigned char   *rid;
    unsigned char   *pix;
    unsigned short  prior;
    struct s_info   *next;
}               app_info;

typedef struct	s_tlv
{
	unsigned char   *t;
	unsigned short  l;
	unsigned char   *v;
	struct s_tlv	*next;
    struct s_tlv    *sub_tlv;
}				t_tlv;

/*utils*/
int             un_strncmp(const char *str1, unsigned const char *str2, size_t n);
int             un_strcmp(const char *str1, const unsigned char *str2);
size_t          un_strlen(const unsigned char *str);
size_t          un_strlcpy(unsigned char *dst, const unsigned char *src, size_t size);
unsigned char   *str_to_ustr(const char *str);
unsigned short  char_to_num(unsigned char first, unsigned char second);
/*init_memory*/
app_info    *init_app();
void    free_apps(app_info **apps);
void    free_1_app(app_info **app);
t_tlv   *init_tlv();
void    free_tlv(t_tlv  **tlv);
void    free_1_tlv(t_tlv **tlv);
/*tag_len_check*/
bool    is_constructed_or_primitiv_tag(unsigned short c);
bool    is_long_tag(unsigned short c);
bool    is_super_long_tag(unsigned short c);
bool    is_len_2(unsigned short c);
bool    is_len_3(unsigned short c);
/*parse_tlv*/
t_tlv   *parse_tlv(unsigned char *pRxBuf);
/*fill_app_data*/
void    fill_apps(t_tlv *tlv, app_info *apps);
/*sort*/
void    sort_apps(app_info **apps);
void    del_n_sup_apps(app_info **apps);
/*print_data*/
void    print_tlv(t_tlv *tlv, short lvl);
void    print_apps(app_info *apps);

#endif