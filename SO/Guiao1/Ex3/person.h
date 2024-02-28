#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "arquivo.txt"

typedef struct person
{
    char name[100];
    int age;
}Person;


int add_person(char *name, int age);

int list_person(int N);

int update_age(char *name, int age);