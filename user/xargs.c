#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // MAXARG

int is_blank(char chr){
    if(chr == ' ' || chr == '\t') return 1;
    else return 0;
} 

int
main(int argCount, char *argValue[])
{
	char buf[2048], ch;
	char *p = buf;
	char *v[MAXARG];
	int c;
	int blanks = 0;
	int offset = 0;

	if(argCount <= 1){
		fprintf(2, "xargs: minimum number of arguments is 2.\n");
		exit(1);
	}

    if(argCount - 1 >= MAXARG) {
         printf("xargs : maximum number of arguments is %d.\n", MAXARG);

    }
	for (c = 1; c < argCount; c++) {
		v[c-1] = argValue[c];
	}
	--c;

	while (read(0, &ch, 1) > 0) {
		if (is_blank(ch)) {
			blanks++;
			continue;
		}

		if (blanks) {  // 之前有过空格
			buf[offset++] = 0;

			v[c++] = p;
			p = buf + offset;

			blanks = 0;
		}

		if (ch != '\n') {
			buf[offset++] = ch;
		} else {
			v[c++] = p;
			p = buf + offset;

			if (!fork()) {
				exit(exec(v[0], v));
			}
			wait(0);
			
			c = argCount - 1;
		}
	}

	exit(0);
}