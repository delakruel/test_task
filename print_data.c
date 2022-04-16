#include "sel_ppse.h"

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