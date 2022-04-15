#include "sel_ppse.h"


// void  fill_app_info(unsigned char *pRxBuf, unsigned short i, app_info *apps, unsigned short app_num)
// {
//     unsigned short  num = 0;
//     unsigned short  l_end;  
//     unsigned short  j;

//     while (num < app_num)
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
//                     j = char_to_num(pRxBuf[i], pRxBuf[i + 1]);
//                     i += 2;
//                     fill_4f(pRxBuf + i, &apps[num], (j - 5) * 2);
//                     i += j * 2;
//                 }
//                 if (pRxBuf[i] == '8' && pRxBuf[i + 1] == '7')
//                 {
//                     i += 4;
//                     fill_87(pRxBuf + i, &apps[num]);
//                     i += 2;
//                 }
//             }
//             ++num;
//         }
//         else
//             ++i;
//     }
//     free(pRxBuf);
// }