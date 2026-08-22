/*
 * impl.h
 *
 * Person class definition and methods.
 */

#ifndef OOP_H
#define OOP_H
#define MAXLEN 20 + 1

typedef struct person {
  char name[MAXLEN], surname[MAXLEN];
  int age;
  struct person *this;

  void (*setName)(struct person *this, const char *name);
  void (*setSurname)(struct person *this, const char *surname);
  void (*setAge)(struct person *this, int age);
  const char *(*toString)(struct person *this);
} Person;

const char *toStringImpl(struct person *this);

void setNameImpl(struct person *this, const char *name);

void setSurnameImpl(struct person *this, const char *surname);

void setAgeImpl(struct person *this, int age);

Person *newPerson(const char *name, const char *surname, int age);

#endif // OOP_H
