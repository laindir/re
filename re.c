#include <stdio.h>
#include <stdlib.h>

typedef struct re_ re;

struct re_
{
	char *(*match)(re *, char *);
	re *lre;
	re *rre;
	char lch;
	char rch;
};

char *literal(re *, char *);
char *concat(re *, char *);
char *altern(re *, char *);
char *star(re *, char *);

char *
literal(re *re, char *str)
{
	if(re->lch == *str)
	{
		return str + 1;
	}
	else
	{
		return NULL;
	}
}

char *
dot(re *re, char *str)
{
	(void)re;

	if(*str != '\n')
	{
		return str + 1;
	}
	else
	{
		return NULL;
	}
}

char *
empty(re *re, char *str)
{
	(void)re;

	return str;
}

char *
concat(re *re, char *str)
{
	char *t = re->lre->match(re->lre, str);

	if(t)
	{
		return re->rre->match(re->rre, t);
	}
	else
	{
		return NULL;
	}
}

char *
altern(re *re, char *str)
{
	char *a = re->lre->match(re->lre, str);
	char *b = re->rre->match(re->rre, str);

	return a > b ? a : b;
}

char *
star(re *re, char *str)
{
	char *a = str;
	char *b = re->lre->match(re->lre, str);

	while(b)
	{
		a = b;
		b = re->lre->match(re->lre, a);
	}

	return a;
}

re pool[100];
int	pp;

re *
new(char *(*match)(re *, char *), re *lre, re *rre, char lch, char rch)
{
	re *r = &pool[pp++];
	r->match = match;
	r->lre = lre;
	r->rre = rre;
	r->lch = lch;
	r->rch = rch;

	return r;
}

re *
stack[100];
int sp;

void
push(re *r)
{
	stack[++sp] = r;
}

re *
pop()
{
	return stack[sp--];
}

re *
top()
{
	return stack[sp];
}

re *
parse(char *regex)
{
	re *p;

	p = new(empty, NULL, NULL, 0, 0);

	for(; *regex; regex++)
	{
		switch(*regex)
		{
		case '*':
			p->rre = new(star, p->rre, NULL, 0, 0);
			break;
		case '(':
			push(new(concat, p, NULL, 0, 0));
			p = new(empty, NULL, NULL, 0, 0);
			break;
		case ')':
			if(top() && top()->match == altern)
			{
				top()->rre = p;
				p = pop();
			}
			top()->rre = p;
			p = pop();
			break;
		case '|':
			if(top() && top()->match == altern)
			{
				top()->rre = p;
				p = pop();
			}
			push(new(altern, p, NULL, 0, 0));
			p = new(empty, NULL, NULL, 0, 0);
			break;
		case '.':
			p = new(concat, p, NULL, 0, 0);
			p->rre = new(dot, NULL, NULL, 0, 0);
			break;
		default:
			p = new(concat, p, NULL, 0, 0);
			p->rre = new(literal, NULL, NULL, *regex, 0);
			break;
		}
	}

	if(top() && top()->match == altern)
	{
		top()->rre = p;
		p = pop();
	}

	return p;
}

char *
match(re *re, char *str, char **endp)
{
	char *e;

	while(str && *str && !e)
	{
		e = re->match(re, str++);
	}

	if(endp)
	{
		*endp = e;
	}

	if(e)
	{
		return str;
	}
	else
	{
		return NULL;
	}
}
