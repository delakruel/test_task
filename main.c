#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

//---------------------------------------------------------------------------------------------
//brief Передает пакет карте и возвращает ответ
//
//param[in] pTxBuf - буфер с пакетом для передаче карте
//param[in] txBufLen - размер пакета карте
//param[out] pRxBuf - буфер, куда будет записан ответный пакет карты
//param[out] rxBufLen - максимальный размер pRxBuf, туда же будет записан размер ответного пакета карты
//
//return true - успех
//return false - ошибка

//bool SendAPDU (unsigned char* pTxBuf, unsigned short txBufSize, unsigned char* pRxBuf, unsigned short* rxBufLen)

typedef struct info
{
    unsigned char   *rid;
    unsigned char   *pix;
    unsigned short  prior;
} app_info;


unsigned short   char_to_num(unsigned char first, unsigned char second)
{
    unsigned short  ret = 0;

    if (second >= '0' && second <= '9')
        ret += second - '0';
    else
        ret += second - 55;
    if (first >= '0' && first <= '9')
        ret += (first - '0') * 16;
    else
        ret += (first - 55) * 16;
    return ret;
}

unsigned short  app_mem(const unsigned char *pRxBuf, unsigned short i, unsigned short end)
{
    unsigned short  num = 0;
    unsigned short  l_end;  

    while (i < end)
    {
        if (pRxBuf[i] == '6' && pRxBuf[i + 1] == '1')
        {
            i += 2;
            l_end = char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + i + 2;
            i += 2;
            while (i < l_end)
            {
                if (!strncmp("9F2A", (pRxBuf + i), 4))
                {
                    i += 4;
                    i += 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;
                }
                if (pRxBuf[i] == '4' && pRxBuf[i + 1] == 'F')
                {
                    i += 2;
                    i += char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + 2;
                    ++num;  
                }
                if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
                {
                    i += 2;
                    i += char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + 2;
                }
            }
        }
        else
            ++i;
    }
    return num;
}

void    fill_4f(const unsigned char *pRxBuf, app_info *cur_app, unsigned short pix_mem)
{
    cur_app->rid = malloc(sizeof(unsigned char) * 11);
    strncpy(cur_app->rid, pRxBuf, 10);
    cur_app->rid[10] = '\0';
    cur_app->pix = malloc(sizeof(unsigned char) * pix_mem + 1);
    strncpy(cur_app->pix, pRxBuf + 10, pix_mem);
    cur_app->pix[pix_mem] = '\0';
}

void    fill_87(const unsigned char *pRxBuf, app_info *cur_app)
{
    if (*(pRxBuf + 1) != '0')
        cur_app->prior = char_to_num('0', *(pRxBuf + 1));
}

void  fill_app_info(const unsigned char *pRxBuf, unsigned short i, app_info *apps, unsigned short app_num)
{
    unsigned short  num = 0;
    unsigned short  l_end;  
    unsigned short  j;

    while (num < app_num)
    {
        if (pRxBuf[i] == '6' && pRxBuf[i + 1] == '1')
        {
            i += 2;
            l_end = char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + i + 2;
            i += 2;
            while (i < l_end)
            {
                if (!strncmp("9F2A", (pRxBuf + i), 4))
                {
                    i += 4;
                    i += 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;
                }
                if (pRxBuf[i] == '4' && pRxBuf[i + 1] == 'F')
                {
                    i += 2;
                    j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
                    i += 2;
                    fill_4f(pRxBuf + i, &apps[num], (j - 5) * 2);
                    i += j * 2;
                }
                if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
                {
                    i += 4;
                    fill_87(pRxBuf + i, &apps[num]);
                    i += 2;
                }
            }
            ++num;
        }
        else
            ++i;
    }
}

void    init_apps_prior(app_info *apps, unsigned short app_num)
{
    for (unsigned short i = 0; i < app_num; ++i)
        apps[i].prior = 16;
}

void    swap_infos(app_info *apps, unsigned short first, unsigned short second)
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

void    print_supported(app_info *apps, unsigned short app_num)
{
    for (unsigned short i = 0; i < app_num; ++i)
        if (!strcmp(apps[i].rid, "A000000658"))
            printf("%s\n%s %s\n", "RID:       PIX:", apps[i].rid, apps[i].pix);
}

int main()
{
    unsigned short  rxBufLen[2] = {510, 122};
    unsigned char   pRxBuf[500] = "6F6C840E325041592E5359532E4444463031A55ABF0C5761094F07A000000658200161134F08A0000006581000039F2A03810643870153610C8701014F07A000000003101061124F07A00000065820028701109F2A0381064361134F08A0000006581000029F2A038106438701529000";

    unsigned short  i;
    unsigned short  end;
    app_info        *apps;
    unsigned short  pix_m;
    unsigned short  app_num;
    
    //printf("%d\n", pRxBuf[122]);
    i = 8 + char_to_num(pRxBuf[6], pRxBuf[7]) * 2;

    if (pRxBuf[i] != 'A' && pRxBuf[i + 1] != '5')
        return 0;

    /* по идее, теги 'A5', '61', '4F' являются одними из обязательных тегов ответа карты на пакет SELECT PPSE, но я не могу пока знать всех нюансов -
    https://www.emvco.com/wp-content/uploads/2017/05/BookB_Entry_Point_Specification_v2_6_20160809023257319.pdf стр. 22, табл. 3-2 */

    i += 2 + 2 + 4;
    end = i + 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;    //до чего идет FCI data 
    i += 2;
    app_num = app_mem(pRxBuf, i, end);  //количество подходящих приложений
    apps = malloc(sizeof(app_info) * app_num);
    init_apps_prior(apps, app_num);
    //printf("%d\n", app_num);
    fill_app_info(pRxBuf, i, apps, app_num);
    sort_app_prior(apps, app_num);
    print_supported(apps, app_num);
    // for (int j = 0; j < app_num; ++j)
    //     printf("%s %s %d\n", apps[j].rid, apps[j].pix, apps[j].prior);
}