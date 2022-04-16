#include "sel_ppse.h"

static unsigned char   *parse_t(unsigned char *pRxBuf, unsigned short *cur)
{
    unsigned short  i = 0;
    unsigned char   *t;

    if (is_long_tag(char_to_num(pRxBuf[*cur + i], pRxBuf[*cur + i + 1])))
    {
        i += 2;
        while (is_super_long_tag(char_to_num(pRxBuf[*cur + i], pRxBuf[*cur + i + 1])))
            i += 2;
    }
    t = malloc(sizeof(unsigned char) * (i + 3));
    un_strlcpy(t, (pRxBuf + *cur), i + 3);
    *cur += i + 2;
    return (t);
}

static unsigned short  parse_l(unsigned char *pRxBuf, unsigned short *cur)
{
    unsigned short  l;

    if (is_len_2(char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1])))
    {
        l = char_to_num(pRxBuf[*cur + 2], pRxBuf[*cur + 3]);
        *cur += 4;
    }
    else if (is_len_3(char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1])))
        {
            l = (char_to_num(pRxBuf[*cur + 2], pRxBuf[*cur + 3]) << 8) + char_to_num(pRxBuf[*cur + 4], pRxBuf[*cur + 5]);
            *cur += 6;
        }
        else
        {
            l = char_to_num(pRxBuf[*cur], pRxBuf[*cur + 1]);
            *cur += 2;
        }
    return l;
}

static unsigned char   *parse_v(unsigned char *pRxBuf, unsigned short len, unsigned short *cur)
{
    unsigned char   *v;

    v = malloc(sizeof(unsigned char) * (len * 2 + 1));
    un_strlcpy(v, (pRxBuf + *cur), len * 2 + 1);
    *cur += len * 2;
    return (v);
}

static void   parse_sub_tlv(unsigned char *pRxBuf, unsigned short len, t_tlv *tlv, unsigned short *cur)
{
    unsigned short  start;
    
    start = *cur;
    len = (len * 2) + *cur;
    while (*cur < len)
    {
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1])) && (*cur - start < tlv->l))
        {
            tlv->sub_tlv = init_tlv();
            tlv = tlv->sub_tlv;
        }
        else
        {
            tlv->next = init_tlv();
            tlv = tlv->next;
        }
        tlv->t = parse_t(pRxBuf, cur);
        tlv->l = parse_l(pRxBuf, cur);
        if (!tlv->l)
            continue ;
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1])))
            parse_sub_tlv(pRxBuf, tlv->l, tlv, cur);
        else
            tlv->v = parse_v(pRxBuf, tlv->l, cur);
    }
}

static void   parse_other(unsigned char *pRxBuf, unsigned short *cur, t_tlv *tlv)
{
    while (*(pRxBuf + *cur) && un_strncmp("9000", (pRxBuf + *cur), 4)) //тег '90' в одних источниках парсится, в других - нет, 
                                                                    //для добавления его в TLV достаточно убрать проверку после &&
    {
        tlv->next = init_tlv();
        tlv = tlv->next;
        tlv->t = parse_t(pRxBuf, cur);
        tlv->l = parse_l(pRxBuf, cur);
        if (!tlv->l)
            continue ;
        if (is_constructed_or_primitiv_tag(char_to_num(tlv->t[0], tlv->t[1]))) 
            parse_sub_tlv(pRxBuf, tlv->l, tlv, cur);
        else
            tlv->v = parse_v(pRxBuf, tlv->l, cur);
    }
}

t_tlv   *parse_tlv(unsigned char *pRxBuf)
{
    t_tlv   *begin;
    unsigned short  i = 0;

    begin = init_tlv();
    begin->t = parse_t(pRxBuf, &i);
    begin->l = parse_l(pRxBuf, &i);
    if (!begin->l)
       parse_other(pRxBuf, &i, begin);
    if (is_constructed_or_primitiv_tag(char_to_num(begin->t[0], begin->t[1])))
        parse_sub_tlv(pRxBuf, begin->l, begin, &i);
    else
        begin->v = parse_v(pRxBuf, begin->l, &i);
    parse_other(pRxBuf, &i, begin);
    return (begin);
}