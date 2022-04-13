#include "sel_ppse.h"

static void    swap_infos(app_info *apps, unsigned short first, unsigned short second)
{
    unsigned char   *tmp_str;
    unsigned short  tmp_num;

    tmp_str = apps[first].pix;
    apps[first].pix = apps[second].pix;
    apps[second].pix = tmp_str;
    tmp_str = apps[first].rid;
    apps[first].rid = apps[second].rid;
    apps[second].rid = tmp_str;
    tmp_num = apps[first].prior;
    apps[first].prior = apps[second].prior;
    apps[second].prior = tmp_num;
}

void    sort_app_prior(app_info *apps, unsigned short app_num)
{
    for (unsigned short i = 1; i < app_num; ++i)
        for (unsigned short j = 0; j < app_num - i; ++j)
            if (apps[j].prior > apps[j + 1].prior)
                swap_infos(apps, j, j + 1);
}