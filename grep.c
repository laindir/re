#include <re.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	re *p;
	char buf[1024];
	FILE *fp;
	int any = 1;

	switch(argc)
	{
	case 3:
		fp = fopen(argv[2], "r");
		if(!fp)
		{
			return 2;
		}
		break;
	case 2:
		fp = stdin;
		break;
	default:
		fputs("Usage: grep <pattern> [<file>]", stderr);
		return 2;
	}

	p = parse(argv[1]);

	while(fgets(buf, 1024, fp))
	{
		if(match(p, buf, NULL))
		{
			any = 0;
			fputs(buf, stdout);
		}
	}

	return any;
}
