#include "sel_ppse.h"

size_t	un_strlen(const unsigned char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	un_strlcpy(unsigned char *dst, const unsigned char *src, size_t size)
{
	size_t	i;

	if (src == NULL)
		return (0);
	if (size == 0)
		return (un_strlen(src));
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (un_strlen(src));
}

int	un_strncmp(const char *str1, const unsigned char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (0);
	while (str1[i] && str2[i] && (i < n - 1) && (str1[i] == str2[i]))
		i++;
	return (str1[i] - str2[i]);
}

int	un_strcmp(const char *str1, const unsigned char *str2)
{
    size_t  i;

    i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
        ++i;
    return (str1[i] - str2[i]);
}

unsigned char   *str_to_ustr(const char *str)
{
    unsigned char   *ustr;
    size_t  i;

    ustr = malloc(sizeof(unsigned char) * (strlen(str) + 1));
    for (i = 0; i < strlen(str); ++i)
        ustr[i] = str[i];
    ustr[i] = '\0';
    return (ustr);
}

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

void    print_supported(app_info *apps, unsigned short app_num)
{
    unsigned short  k = 1;
    for (unsigned short i = 0; i < app_num; ++i)
        if (!un_strncmp("A000000658", apps[i].rid, 10))
        {
            printf("%d%s\n%s %s\n", k, " RID:     PIX:", apps[i].rid, apps[i].pix);
            ++k;
        }
    if (k == 1)
        printf("%s\n", "No supported applications!");
}