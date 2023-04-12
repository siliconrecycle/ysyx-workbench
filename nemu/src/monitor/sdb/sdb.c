#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(demu) ");

  if (line_read && *line_read) { // 判断line_read指针是否为空，判断line_read的第一个字符是否为空。
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1); // -1 意味着传入的是无符号最大的数,这种技巧最好不用,容易导致理解困难.
	return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_hex(char *args);
static int cmd_expr(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "si", "Step instruction", cmd_si },
  { "info", "info register/watchpoint state", cmd_info },
  { "x", "hex N vaddr", cmd_hex },
  { "p", "p [exprt]: 计算表达式。", cmd_expr },
};

static int cmd_expr(char *args) {
	bool ok = false;
	bool *okp = &ok;

	char str[1000] = {}, *arg;
	
	while ((arg = strtok(NULL, " ")) > 0) {
		/* printf("cmd_expr: arg: <%s>\n", arg); */
		strcat(str, arg);
	}
	printf("cmd_expr: str: <%s>\n", str);

	expr(str, okp);
	if (!*okp)
		printf("cmd_expr: error: expr\n");

	return 0;
}

#include <memory/vaddr.h>
#include <math.h>

static word_t parse(char *cmd) {
        char *str = cmd, c;

        strsep(&str, "x");
//        printf("%s\n", str);

        int s = 0, len = strlen(str), i;
        for (i = 0; len > 0; len--, i++) {
                int r = 0;
                c = str[i];
//                printf("%c\n", c);
                c = tolower(c);
                if (isalpha(c) && c < 'g') {
                        r += 9 + (c - 'a');
                } else if (isdigit(c))
                        r += (c - '0');
                else {
                        printf("unknow hex!\n");
                        return 0x80000000;
                }
                r *= pow(16, len);
                s += r;
        }
//        printf("%d\n", s);

        return (word_t)s;
}

static int cmd_hex(char *args) {
	int n = 1;
	word_t addr, mem;

	char *arg1 = strtok(NULL, " ");

	if (arg1 == NULL) {
		printf("unknown cmd1!\n");
                return 0;
        }
	n = atoi(arg1);

	char *arg2 = strtok(NULL, " ");
	if (arg2 == NULL) {
		printf("unknown cmd2!\n");
                return 0;
	}
        addr = parse(arg2);

        while (n-- > 0) {
        	mem = vaddr_read(addr, 4);
	        printf( "0x%016lx\t0x%08lx\n", addr, mem);
                addr += 4;
        }
	return 0;
}

static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");
	if (arg == NULL) {
		printf("unknown cmd!\n");
		return 0;
	}
	if (strcmp(arg, "r") == 0) {
//		printf("%s\n", arg);
		isa_reg_display();
	} else if (strcmp(arg, "w") == 0)
		printf("%s\n", arg);
	else
		printf("unknown cmd!\n");
	return 0;
}

static int cmd_si(char *args) {
	int step = 1;
	char *arg = strtok(NULL, " ");
	if (arg != NULL)
		step = atoi(arg);
	cpu_exec(step);
	return 0;
}

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " "); // strtok 将str以空格为分隔符号，提取token。
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
