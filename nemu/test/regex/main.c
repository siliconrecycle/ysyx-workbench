#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

static const char *const str =
	"1) John Driverhacker;\n2) John Doe;\n3) John Foo;\n";

static const char *const re = "John.*o";

int main(void)
{
	static const char *s = str;
	regex_t     regex;
	regmatch_t  pmatch[1];
	regoff_t    off, len;

	if (regcomp(&regex, re, REG_NEWLINE))
		exit(EXIT_FAILURE);

	printf("String = \n\"%s\"\n", str);
	printf("Matches:\n");

	for (int i = 0; ; i++) {
		if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
			break;

		printf("s - str: %d\n", s - str);
		off = pmatch[0].rm_so + (s - str);
//		off = pmatch[0].rm_so;              // 它的开始的偏移，计算的是从上次开始的地方来计算的。比如上次开始的地方是 27，那么这次开始的偏移就是对 27 的偏移，也就是 8.
		len = pmatch[0].rm_eo - pmatch[0].rm_so;
		printf("#%d:\n", i);
		printf("offset = %jd; length = %jd\n", (intmax_t) off,
                       (intmax_t) len);
		printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so); // 这个打印也是非常有意思的，和往常的不同。

		s += pmatch[0].rm_eo;
	}

	exit(EXIT_SUCCESS);
}


