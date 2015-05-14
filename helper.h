#include <sys/time.h>
void print_error();
void print_special_error(char * arg);
void handle_error();
char ** handle_args(char * cmd, char * args);
long timevaldiff(struct timeval * start, struct timeval * end);
