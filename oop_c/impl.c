/*
 * impl.c
 *
 * Person class methods implementation.
 */

#define ILLEGAL_ARG_ERR -1
#define NULL_PTR_ERR -2

#include "impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *toStringImpl(struct person *this) {
  if (this == NULL)
    exit(NULL_PTR_ERR);
  char *res = (char *)malloc(MAXLEN * 10 * sizeof(char));

  if (res == NULL)
    exit(NULL_PTR_ERR);

  res[0] = '\0';
  strcat(res, "name= ");
  strcat(res, this->name);
  strcat(res, ", ");
  strcat(res, "surname= ");
  strcat(res, this->surname);
  strcat(res, ", ");

  char age[MAXLEN];
  sprintf(age, "%d", this->age);

  strcat(res, "age= ");
  strcat(res, age);
  strcat(res, "\n");
  return res;
}

void setNameImpl(struct person *this, const char *name) {
  if (this == NULL)
    exit(NULL_PTR_ERR);
  else if (strlen(name) > MAXLEN)
    exit(ILLEGAL_ARG_ERR);

  strcpy(this->name, name);
}

void setSurnameImpl(struct person *this, const char *surname) {
  if (this == NULL)
    exit(NULL_PTR_ERR);
  else if (strlen(surname) > MAXLEN)
    exit(ILLEGAL_ARG_ERR);

  strcpy(this->surname, surname);
}

void setAgeImpl(struct person *this, int age) {
  if (this == NULL)
    exit(NULL_PTR_ERR);

  this->age = age;
}

Person *newPerson(const char *name, const char *surname, int age) {
  Person *p = (Person *)malloc(sizeof(Person));

  if (p == NULL)
    exit(NULL_PTR_ERR);

  if (strlen(name) > MAXLEN || strlen(surname) > MAXLEN)
    exit(ILLEGAL_ARG_ERR);

  strcpy(p->name, name);
  strcpy(p->surname, surname);
  p->age = age;
  p->this = p;

  p->toString = &toStringImpl;
  p->setName = &setNameImpl;
  p->setSurname = &setSurnameImpl;
  p->setAge = &setAgeImpl;

  return p;
}
