#include "sel_ppse.h"

int main(int argc, char **argv)
{
    app_info        *apps;
    unsigned char   *pRxBuf;
    unsigned short  i;
    unsigned short  end;
    unsigned short  app_num;

    if (argc != 2)
    {
        printf("%s\n", "Enter 1 argument!");
        return 0;
    }
    pRxBuf = str_to_ustr(argv[1]);
    i = 8 + char_to_num(pRxBuf[6], pRxBuf[7]) * 2;
    if (pRxBuf[i] != 'A' && pRxBuf[i + 1] != '5')
    {
        printf("%s\n", "No FCI");
        return 0;
    }
    i += 8;
    end = i + 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2; 
    i += 2;
    app_num = app_mem(pRxBuf, i, end);
    apps = malloc(sizeof(app_info) * app_num);
    init_apps_prior(apps, app_num);
    fill_app_info(pRxBuf, i, apps, app_num);
    sort_app_prior(apps, app_num);
    print_supported(apps, app_num);
    free_all(apps, app_num);
    return 0;
}