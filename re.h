#ifndef RE_H
#define RE_H

typedef struct re_ re;

re *parse(char *regex);

char *match(re *re, char *str, char **endp);

#endif
