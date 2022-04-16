#include "sel_ppse.h"

int     main(int argc, char **argv)
{
    app_info        *apps;
    unsigned char   *pRxBuf;
    t_tlv           *tlv;

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
    free(pRxBuf);
    print_tlv(tlv, 0);
    apps = init_app();
    fill_apps(tlv, apps);
    free_tlv(&tlv);
    del_n_sup_apps(&apps);
    sort_apps(&apps);
    print_apps(apps);
    free_apps(&apps);
    return 0;
}