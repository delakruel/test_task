#include "sel_ppse.h"

static void    fill_4f(unsigned char *val_4f, app_info *cur_app, size_t pix_mem)
{
    cur_app->rid = malloc(sizeof(unsigned char) * 11);
    un_strlcpy(cur_app->rid, val_4f, 11);
    cur_app->pix = malloc(sizeof(unsigned char) * pix_mem + 1);
    un_strlcpy(cur_app->pix, val_4f + 10, pix_mem + 1);
}

static void    fill_87(unsigned char *val_87, app_info *cur_app)
{
    if (*(val_87 + 1) != '0')
        cur_app->prior = char_to_num('0', *(val_87 + 1));
}

static void    fill_1_app(t_tlv *tlv, app_info *apps)
{
    static bool f = 0;

    if (f)
    {
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

void    fill_apps(t_tlv *tlv, app_info *apps)
{
    while (tlv != NULL)
    {
        if (!un_strcmp("61", tlv->t) && tlv->sub_tlv)
            fill_1_app(tlv->sub_tlv, apps);
        if (tlv->sub_tlv)
            fill_apps(tlv->sub_tlv, apps);
        tlv = tlv->next;
    }
}