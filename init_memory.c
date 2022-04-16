#include "sel_ppse.h"

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

void    free_1_app(app_info **app)
{
    if ((*app)->pix)
        free((*app)->pix);
    if ((*app)->rid)
        free((*app)->rid);
    free(*app);
    (*app) = NULL;
}

void    free_apps(app_info **apps)
{
    app_info    *tmp;
    app_info    *beg;

    tmp = *apps;
    while (tmp->next)
    {
        beg = tmp;
        tmp = tmp->next;
        free_1_app(&beg);
    }
    free_1_app(&tmp);
    (*apps) = NULL;
}

void    free_1_tlv(t_tlv **tlv)
{
    if ((*tlv)->t)
        free ((*tlv)->t);
    if ((*tlv)->v)
        free ((*tlv)->v);
    free (*tlv);
    (*tlv) = NULL;
}

void    free_tlv(t_tlv  **tlv)
{
    t_tlv   *tmp;
    t_tlv   *beg;

    tmp = *tlv;
    while (tmp->next)
    {
        beg = tmp;
        if (tmp->sub_tlv)
            free_tlv(&(tmp->sub_tlv));
        tmp = tmp->next;
        free_1_tlv(&beg);
    }
    {
        if (tmp->sub_tlv)
            free_tlv(&(tmp->sub_tlv));
        free_1_tlv(&tmp);
    }
    (*tlv) = NULL;
}
