#include "sel_ppse.h"

static void    swap_with_next(app_info *app)
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
    app_info    *i, *j;

    j = (*apps)->next;
    i = (*apps);
    while (j != NULL)
    {
        while (i->next != NULL)
        {
            if (i->prior > i->next->prior)
                swap_with_next(i);
            i = i->next;
        }
        i = (*apps);
        j = j->next;
    }
}

void    del_n_sup_apps(app_info **apps)
{
    app_info    *prev_elem, *cur, *tmp;

    prev_elem = NULL;
    cur = *apps;
    while (cur != NULL)
    {
        if (un_strcmp("A000000658", cur->rid))
        {
            if (prev_elem != NULL)
                prev_elem->next = cur->next;
            else
                (*apps) = cur->next;
            tmp = cur->next;
            free_1_app(&cur);
            cur = tmp;
        }
        else
        {
            prev_elem = cur;
            cur = cur->next;
        }
    }
}