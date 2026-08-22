#include "impl.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  printf("Creating new Person instance (john, white, 20):\n");
  Person *person = newPerson("john", "white", 20);
  printf("toString: \n%s", person->toString(person->this));

  printf("Setting new name, surname and age (jane, doe, 18):\n");
  person->setName(person->this, "jane");
  person->setSurname(person->this, "doe");
  person->setAge(person->this, 18);

  printf("toString: \n%s", person->toString(person->this));

  free(person);

  return EXIT_SUCCESS;
}
