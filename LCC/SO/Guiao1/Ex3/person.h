#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define FILENAME "file_pessoas"

typedef struct person{
    char name[100];
    int age;
} Person;


int add_person(char *name, int age);

int list_person(int N);

int update_age(char *name, int age);

int update_age_V2(int pos, int age);