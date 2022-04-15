#include "sel_ppse.h"

// unsigned short  app_mem(unsigned char *pRxBuf, unsigned short i, unsigned short end)
// {
//     unsigned short  num = 0;
//     unsigned short  l_end;  

//     while (i < end)
//     {
//         if (pRxBuf[i] == '6' && pRxBuf[i + 1] == '1')
//         {
//             i += 2;
//             l_end = char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + i + 2;
//             i += 2;
//             while (i < l_end)
//             {
//                 if (!un_strncmp("9F2A", (pRxBuf + i), 4))
//                 {
//                     i += 4;
//                     i += 2 + char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2;
//                 }
//                 if (pRxBuf[i] == '4' && pRxBuf[i + 1] == 'F')
//                 {
//                     i += 2;
//                     i += char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + 2;
//                     ++num;  
//                 }
//                 if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
//                 {
//                     i += 2;
//                     i += char_to_num(pRxBuf[i], pRxBuf[i + 1]) * 2 + 2;
//                 }
//             }
//         }
//         else
//             ++i;
//     }
//     return num;
// }

void    free_all(app_info *apps, unsigned short app_num)
{
    for (unsigned short i = 0; i < app_num; ++i)
    {
        free(apps[i].pix);
        free(apps[i].rid);
    }
    free(apps);
}


