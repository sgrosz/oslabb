#include <sys/time.h>
void print_error(char * message);
void handle_error(int err, char * message);
char ** handle_args(char * cmd, char * args);
long timevaldiff(struct timeval * start, struct timeval * end);
