#include <stdio.h>
//#include <termios.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <readline/readline.h>
//#include <readline/history.h>
//
//int main(void) {
//	static char *line_read = NULL;
//
//	if (line_read) {
//		free(line_read);
//		line_read = NULL;
//	}
//
//	/* line_read = readline("(nume) "); */
//	line_read = readline("(test) "); // this is no prompt.
//
//	char *arg = strtok(line_read, " ");
//	if (arg == NULL) {
//		printf("%s\n", arg);
//		return 0;
//	}
//	char c = arg[0];
//	switch (c) {
//	case 'b':
//		printf("input is b.\n");
//		break;
//	case 'n':
//		printf("input is n,\n");
//		break;
//	default:
//		printf("unknow input.\n");
//		break;
//	}
//
//	printf("termianl is over!\n");
//}


union number {
	int i;
	float f;
	double d;
	char c;
};

int main() {
    	union number n;
    	n.i = 48;
    	printf("i = %d\n", n.i);
	printf("f = %f\n", n.f);
    	printf("d = %lf\n", n.d);
    	printf("c = %c\n", n.c);
	printf("-----------------\n");

	n.f = 3.14;
    	printf("f = %f\n", n.f);
    	printf("i = %d\n", n.i);
    	printf("d = %lf\n", n.d);
    	printf("c = %c\n", n.c);
	printf("-----------------\n");

	n.d = 2.71828;
    	printf("d = %lf\n", n.d);
    	printf("i = %d\n", n.i);
    	printf("f = %f\n", n.f);
    	printf("c = %c\n", n.c);
	printf("-----------------\n");

	n.c = 'A';
    	printf("c = %c\n", n.c);
    	printf("i = %d\n", n.i);
    	printf("f = %f\n", n.f);
    	printf("d = %lf\n", n.d);

    	return 0;
}
