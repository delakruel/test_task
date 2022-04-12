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
    unsigned char   rid[10];
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
    unsigned short  j;

    while (i < end)
    {
        if (pRxBuf[i] == '6' && pRxBuf[i + 1] == '1')//здесь уже в любом случае будет 61, нет смысла проверять, но на всякий
        {
            i += 2;
            l_end = char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + i + 2;
                // printf("%c", pRxBuf[l_end]);
            i += 2;
            while (i < l_end)
            {
                if (pRxBuf[i] == '9' && pRxBuf[i + 1] == 'F' && pRxBuf[i + 2] == '2' && pRxBuf[i + 3] == 'A')
                {
                    i += 4;
                    i += 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;
                }
                if (pRxBuf[i] == '4' && pRxBuf[i + 1] == 'F')
                {
                    i += 2;
                    j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
                    i += 2;
                    if (pRxBuf[i] == 'A' && pRxBuf[i + 1] == '0' && pRxBuf[i + 2] == '0' && pRxBuf[i + 3] == '0' && pRxBuf[i + 4] == '0' && 
                       pRxBuf[i + 5] == '0' && pRxBuf[i + 6] == '0' && pRxBuf[i + 7] == '6' && pRxBuf[i + 8] == '5' && pRxBuf[i + 9] == '8')
                        ++num; // выбираем только поддерживаемые ридером приложения 
                    i += j * 2;
                }
                if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
                {
                    i += 2;
                    j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
                    i += j * 2 + 2;
                }
            }
        }
        else
            ++i;
    }
    return num;
}

unsigned short  fill_app_info(const unsigned char *pRxBuf, unsigned short i, unsigned short end, app_info *kek)
{
    unsigned short  num = 0;
    unsigned short  l_end;  
    unsigned short  j;
    
    //kek[0].rid[0] = 'F';

    while (i < end)
    {
        if (pRxBuf[i] == '6' && pRxBuf[i + 1] == '1')
        {
            i += 2;
            l_end = char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + i + 2;
            i += 2;
            while (i < l_end)
            {
                if (pRxBuf[i] == '9' && pRxBuf[i + 1] == 'F' && pRxBuf[i + 2] == '2' && pRxBuf[i + 3] == 'A')
                {
                    i += 4;
                    i += 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;
                }
                if (pRxBuf[i] == '4' && pRxBuf[i + 1] == 'F')
                {
                    i += 2;
                    j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
                    i += 2;
                    if (pRxBuf[i] == 'A' && pRxBuf[i + 1] == '0' && pRxBuf[i + 2] == '0' && pRxBuf[i + 3] == '0' && pRxBuf[i + 4] == '0' && 
                       pRxBuf[i + 5] == '0' && pRxBuf[i + 6] == '0' && pRxBuf[i + 7] == '6' && pRxBuf[i + 8] == '5' && pRxBuf[i + 9] == '8')
                        ////////////////
                    i += j * 2;
                }
                if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
                {
                    i += 2;
                    j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
                    i += j * 2 + 2;
                }
            }
        }
        else
            ++i;
    }
    return num;
}

int main()
{
    unsigned short  rxBufLen[2] = {510, 122};
    unsigned char   pRxBuf[122] = "6F39840E325041592E5359532E4444463031A527BF0C2461129F2A038106438701034F07A0000004320001610E8701024F09A000000658101033339000";
    //unsigned char   pRxBuf[72] = "6F20840E325041592E5359532E4444463031A50EBF0C0B61099F2A038106438701019000";

    unsigned short  j;
    unsigned short  i;
    unsigned short  end;
    app_info        *apps;
    unsigned short  pix_m;
    unsigned short  app_num;
    
    i = 8 + char_to_num(pRxBuf[6], pRxBuf[7]) * 2;

    if (pRxBuf[i] != 'A' && pRxBuf[i + 1] != '5')
        return 0;

    /* по идее, теги 'A5', '61', '4F' является одними из обязательных тегов ответа карты на пакет SELECT PPSE, но я не могу пока знать всех нюансов -
    https://www.emvco.com/wp-content/uploads/2017/05/BookB_Entry_Point_Specification_v2_6_20160809023257319.pdf стр. 22, табл. 3-2 */

    i += 2 + 2 + 4;
    end = i + 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;    //до чего идет FCI data 
    i += 2;
    app_num = app_mem(pRxBuf, i, end);  //количество подходящих приложений
    apps = malloc(sizeof(app_info) * app_num);
    // printf("%d", sizeof(app_info));
    // for (j = 0; j < app_num; ++j)
    fill_app_info(pRxBuf, i, end, apps);
    printf("%s", apps[0].rid);
}