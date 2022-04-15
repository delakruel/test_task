#include "sel_ppse.h"

typedef struct	s_tlv
{
	unsigned char   *t;
	unsigned short  l;
	unsigned char   *v;
	struct s_tlv	*next;
    struct s_tlv    *sub_tlv;
}				t_tlv;

bool    is_long_tag(unsigned short c)
{
    if ((c & 0b00011111) == 0b00011111)
        return 1;
    return 0;
}

bool    is_super_long_tag(unsigned short c)
{
    if (c & 0b10000000)
        return 1;
    return 0;
}

unsigned char   *parse_t(unsigned char *pRxBuf, unsigned short *cur)
{
    unsigned short  i = 0;
    unsigned char   *t;

    if (is_long_tag(char_to_num(pRxBuf[*cur + i], pRxBuf[*cur + i + 1])))
    {
        i += 2;
        while (is_super_long_tag(char_to_num(pRxBuf[*cur + i], pRxBuf[*cur + i + 1])))
            i += 2;
    }
    t = malloc(sizeof(unsigned char) * (i + 3));
    un_strlcpy(t, (pRxBuf + *cur), i + 3);
    *cur += i + 2;
    return (t);
}

bool    is_len_2(unsigned short c)
{
    if ((c & 0b10000001) == 0b10000001)
        return 1;
    return 0;
}

bool    is_len_3(unsigned short c)
{
    if ((c & 0b10000010) == 0b10000010)
        return 1;
    return 0;
}

unsigned short  parse_l(unsigned char *pRxBuf, unsigned short *cur)
{
    unsigned short  l;

    if (is_len_2(char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1])))
    {
        l = char_to_num(pRxBuf[*cur + 2], pRxBuf[*cur + 3]);
        *cur += 4;
    }
    else if (is_len_3(char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1])))
        {
            l = (char_to_num(pRxBuf[*cur + 2], pRxBuf[*cur + 3]) << 8) + char_to_num(pRxBuf[*cur + 4], pRxBuf[*cur + 5]);
            *cur += 6;
        }
        else
        {
            l = char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1]);
            *cur += 2;
        }
    return l;
}

t_tlv   *init_tlv()
{
    t_tlv   *new_tlv;

    new_tlv = malloc(sizeof(t_tlv));
    new_tlv->next = NULL;
    new_tlv->t = NULL;
    new_tlv->l = 0;
    new_tlv->v = NULL;
    new_tlv->sub_tlv = NULL;
    return (new_tlv);
}

bool    is_constructed_or_primitiv_tag(unsigned short c)
{
    if (c & 0b00100000)
        return 1;
    return 0;
}

unsigned char   *parse_v(unsigned char *pRxBuf, unsigned short len, unsigned short *cur)
{
    unsigned char   *v;

    v = malloc(sizeof(unsigned char) * (len * 2 + 1));
    un_strlcpy(v, (pRxBuf + *cur), len * 2 + 1);
    *cur += len * 2;
    return (v);
}

void   parse_sub_tlv(unsigned char *pRxBuf, unsigned short len, t_tlv *tlv, unsigned short *cur)
{
    len = (len * 2) + *cur;

    unsigned short  start = *cur;

    while (*cur < len)
    {
    // printf("%c ", pRxBuf[*cur]);
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1])) && (*cur - start < tlv->l))
        {
        // printf("%s SUB ", tlv->t);//
            tlv->sub_tlv = init_tlv();
            tlv = tlv->sub_tlv;
        }
        else
        {
            // printf("%s NEXT ", tlv->t);
            tlv->next = init_tlv();
            tlv = tlv->next;
        }
        // printf(" cur %c ", pRxBuf[*cur]);
        tlv->t = parse_t(pRxBuf, cur);
        // printf("%s ", tlv->t);
        tlv->l = parse_l(pRxBuf, cur);
        if (!tlv->l)
            continue ;
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1])))
            parse_sub_tlv(pRxBuf, tlv->l, tlv, cur);
        else
            tlv->v = parse_v(pRxBuf, tlv->l, cur);
        // if (tlv->v) 
        // printf("%s\n", tlv->v);
    }
}

void   parse_other(unsigned char *pRxBuf, unsigned short *cur, t_tlv *tlv)
{
    while (*(pRxBuf + *cur) && un_strncmp("9000", (pRxBuf + *cur), 4)) //тег '90' в одних источниках парсится, в других - нет, 
                                                                    //для добавления его в TLV достаточно убрать проверку после &&
    {
        // printf("222");//
        tlv->next = init_tlv();
        tlv = tlv->next;
        tlv->t = parse_t(pRxBuf, cur);
        tlv->l = parse_l(pRxBuf, cur);
        if (!tlv->l)
            continue ;
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1]))) 
            parse_sub_tlv(pRxBuf, tlv->l, tlv, cur);
        else
            tlv->v = parse_v(pRxBuf, tlv->l, cur);
    }
}

t_tlv   *parse_tlv(unsigned char *pRxBuf)
{
    t_tlv   *begin;
    unsigned short  i = 0;
    begin = init_tlv();
    begin->t = parse_t(pRxBuf, &i);
    begin->l = parse_l(pRxBuf, &i);
    if (!begin->l)
       parse_other(pRxBuf, &i, begin);
    if (is_constructed_or_primitiv_tag(char_to_num(begin->t[0], begin->t[1])))
        parse_sub_tlv(pRxBuf, begin->l, begin, &i);
    else
        begin->v = parse_v(pRxBuf, begin->l, &i);
    parse_other(pRxBuf, &i, begin);
    free (pRxBuf);
    return (begin);
}

void    print_tlv(t_tlv *tlv, short lvl)
{
    short old_lvl = lvl;

    while (tlv != NULL)
    {
        while (lvl--)
            write(1, "  ", 2);
        printf("T: %s\n", tlv->t);
        lvl = old_lvl;
        while (lvl--)
            write(1, "  ", 2);
        printf("L: %d\n", tlv->l);
        lvl = old_lvl;
        while (lvl--)
            write(1, "  ", 2);
        if (tlv->v)
            printf("V: %s\n", tlv->v);
        else
            write(1, "V:\n", 3);
        lvl = old_lvl;
        if (tlv->sub_tlv != NULL)
            print_tlv(tlv->sub_tlv, lvl + 1);
        tlv = tlv->next;
    }
}

// unsigned short  app_mem(t_tlv *tlv)
// {
//     unsigned short  num = 0;
    
//     while (tlv != NULL)
//     {
//         if (!un_strcmp("4F", tlv->t) && !un_strncmp("A000000658", tlv->v, 10))
//             ++num;
//         if (tlv->sub_tlv != NULL)
//             num += app_mem (tlv->sub_tlv);
//         tlv = tlv->next;
//     }

//     return num;
// }

static void    fill_4f(unsigned char *val_4f, app_info *cur_app, size_t pix_mem)
{
    // printf(" %lu ", pix_mem);//
    cur_app->rid = malloc(sizeof(unsigned char) * 11);
    un_strlcpy(cur_app->rid, val_4f, 11);
    cur_app->pix = malloc(sizeof(unsigned char) * pix_mem + 1);
    un_strlcpy(cur_app->pix, val_4f + 10, pix_mem + 1);
}

static void    fill_87(unsigned char *pRxBuf, app_info *cur_app)
{
    if (*(pRxBuf + 1) != '0')
        cur_app->prior = char_to_num('0', *(pRxBuf + 1));
}

app_info    *init_app()
{
    app_info    *new_app;

    new_app = malloc(sizeof(app_info));
    new_app->rid = NULL;
    new_app->pix = NULL;
    new_app->next = NULL;
    new_app->prior = 16;

    return (new_app);
} 

void    fill_2(t_tlv *tlv, app_info *apps)
{
    static bool f = 0;
    // printf(" fill2 ");//
    if (f)
    {
        // printf("new ");
        while (apps->next)
            apps = apps->next;
        apps->next = init_app();
        apps = apps->next;
    }
    while (tlv != NULL)
    {
        if (!un_strcmp("4F", tlv->t))
            fill_4f(tlv->v, apps, un_strlen(tlv->v) - 10);
        if (!un_strcmp("87", tlv->t))
            fill_87(tlv->v, apps);

        tlv = tlv->next;
    }
    f = 1;
}

void    fill_1(t_tlv *tlv, app_info *apps)
{
    while (tlv != NULL)
    {
        if (!un_strcmp("61", tlv->t) && tlv->sub_tlv)
            fill_2(tlv->sub_tlv, apps);

        
        
        if (tlv->sub_tlv)
            fill_1(tlv->sub_tlv, apps);
        tlv = tlv->next;
    }
    //free(tlv);
}

void    print_apps(app_info *apps)
{
    unsigned short  k = 1;

    while (apps != NULL)
    {
        printf("%d RID:     PIX:\n", k);
        if (apps->rid)
            printf("%s ", apps->rid);
        if (apps->pix)
            printf("%s ", apps->pix);
        apps = apps->next;
        printf("\n");
        ++k;
    }
    if (k == 1)
        printf("No supported apps!\n");
}

void    del_n_sup_apps(app_info **apps)
{
    app_info    *prev_elem = NULL;
    app_info    *tmp = *apps;

    while (tmp != NULL)
    {
        if (un_strcmp("A000000658", tmp->rid))
        {
            if (prev_elem != NULL)
                prev_elem->next = tmp->next;
            else
                *apps = tmp->next;
            {
                free(tmp->pix);
                free(tmp->rid);
                free(tmp);
            }
        }
        prev_elem = tmp;
        tmp = tmp->next;
    }
}

// void    sort_app_prior(app_info *apps, unsigned short app_num)
// {
//     for (unsigned short i = 1; i < app_num; ++i)
//         for (unsigned short j = 0; j < app_num - i; ++j)
//             if (apps[j].prior > apps[j + 1].prior)
//                 swap_infos(apps, j, j + 1);
// }

void    swap_with_next(app_info *app)
{
    unsigned char   *tmp_str;
    unsigned short  tmp_num;

    tmp_str = app->rid;
    app->rid = app->next->rid;
    app->next->rid = tmp_str;

    tmp_str = app->pix;
    app->pix = app->next->pix;
    app->next->pix = tmp_str;

    tmp_num = app->prior;
    app->prior = app->next->prior;
    app->next->prior = tmp_num;
}

void    sort_apps(app_info **apps)
{
    app_info    *tmp, *begin, *temp;
    app_info    *i, *j;



    j = (*apps)->next;
    i = (*apps);

    while (j != NULL)
    {
        while (i->next != NULL)
        {
            if (i->prior > i->next->prior)
            {
                swap_with_next(i);
            }
            i = i->next;
        }
        i = (*apps);
        j = j->next;
    }
}

int     main(int argc, char **argv)
{
    app_info        *apps;
    unsigned char   *pRxBuf;
    t_tlv           *tlv;
    // unsigned short  i;
    // unsigned short  end;
    unsigned short  app_num;

    if (argc != 2)
    {
        printf("%s\n", "Enter 1 argument!");
        return 0;
    }
    if (argv[1][0] == '\0')
    {
        printf("%s\n", "Enter non-empty argument!");
        return 0;
    }
    pRxBuf = str_to_ustr(argv[1]);
    tlv = parse_tlv(pRxBuf);
    // print_tlv(tlv, 0);
    // print_tlv(tlv, 0);
    // app_num = app_mem(tlv);
    //printf("%d\n", app_num);
    apps = init_app();
    fill_1(tlv, apps);
    // print_apps(apps);
    del_n_sup_apps(&apps);
    //if (apps)
    sort_apps(&apps);
    print_apps(apps);
    // apps = malloc(sizeof(app_info) * app_num);
    // init_apps_prior(apps, app_num);
    // i = 8 + char_to_num(pRxBuf[6], pRxBuf[7]) * 2;
    // if (pRxBuf[i] != 'A' && pRxBuf[i + 1] != '5')
    // {
    //     printf("%s\n", "No FCI");
    //     return 0;
    // }
    // i += 8;
    // end = i + 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2; 
    // i += 2;
    // app_num = app_mem(pRxBuf, i, end);
    // apps = malloc(sizeof(app_info) * app_num);
    // init_apps_prior(apps, app_num);
    // fill_app_info(pRxBuf, i, apps, app_num);
    // sort_app_prior(apps, app_num);
    // print_supported(apps, app_num);
    // free_all(apps, app_num);
    return 0;
}