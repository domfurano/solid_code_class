#include <stdio.h>
#include <stdarg.h>
#include "musl.h"
#define LEN 10000
char buf[LEN];
int main(int argc, const char *argv[]) {
musl_snprintf(buf, LEN, "%s" ,  " Hello world \n " );
)printf("%s", buf);return 0;
}