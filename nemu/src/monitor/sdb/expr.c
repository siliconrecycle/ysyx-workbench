#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,

	/* TODO: Add more token types */
	TK_LEFT, TK_RIGHT,
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" ", TK_NOTYPE},    // spaces
  {"\\+", TK_PLUS},         // plus
  {"==", TK_EQ},        // equal
  {"[0-9][^+-\\*\\/\\(\\)]*", TK_NUM},        // 数字
  {"\\-", TK_MINUS},        // 减
  {"\\*", TK_MUL},        // 乘
  {"\\/", TK_DIV},        // 除
  {"\\(", TK_LEFT},        // 左括号
  {"\\)", TK_RIGHT},        // 右括号
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

	Token t = {0, ""};
	t.type = rules[i].token_type;
	strncpy(t.str, substr_start, substr_len);
	
        switch (rules[i].token_type) {
	case TK_PLUS:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_MINUS:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_MUL:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_DIV:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_LEFT:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_RIGHT:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_NOTYPE:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	case TK_NUM:
	  printf("make_token: <%s>\n", substr_start);
	  tokens[nr_token++] = t;
	  printf("make_token: tokens[%d] = <%s>\n", nr_token - 1, tokens[nr_token - 1].str);
	  break;
	default:
	  printf("error: unknown operator!\n");
	  break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");

      return false;
    }
  }

  return true;
}

void prftokens(Token t) {
	printf("token: [");
	printf("type: %d, str: %s", t.type, t.str);
	printf("]\n");
}

int checkparenthesis(int s, int e) {
	TODO();
}

word_t eval(int s, int e) {
	if (s > e)
		return;
	else if (s == e)
		return tokens[e];
	else {
		int checkpth = checkparenthesis(s, e);
		if (checkpth == 1)
			eval(s + 1, e - 1);
		else if (checkpth == 0)
			

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */

  int i, r;

  // 1. get the number of tokens.
  printf("num: %ld / %ld\n", sizeof(tokens), sizeof(Token));
  for (i = 0; i < 32; i++) {
	  prftokens(tokens[i]);
	  if ((tokens[i].type != TK_NUM) && (tokens[i].type != TK_PLUS)
	      && (tokens[i].type != TK_MINUS) && (tokens[i].type != TK_MUL)
	      && (tokens[i].type != TK_DIV))
		  break;
  }
  printf("expr: num of token: <%d>\n", i);

  // 2. eval(start, end) of the expr.
  r = eval(0, i);
  
  return 0;
}
