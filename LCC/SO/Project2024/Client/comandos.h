#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUF_SIZE 300

int comando_execute(char*, char*, char*);
int comando_end();
int comando_status();
