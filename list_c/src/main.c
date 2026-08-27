#define LIST_TYPE 
#include "list.h"

#define BUFFER_SIZE 100
DEFINE_LIST(float);

typedef struct person_t {
	const char* name;
	unsigned age;
} person;

DEFINE_LIST(person);

const char* float_to_string(const float* item) {
	char* buffer = QALLOC(char, BUFFER_SIZE);
	CHECK_ALLOC(buffer, "[ERROR] float to string buffer alloc");
	snprintf(buffer, BUFFER_SIZE, "%.3f", *item);
	return buffer;
}

const char* person_to_string(const person* item) {
	char* buffer = QALLOC(char, BUFFER_SIZE);
	CHECK_ALLOC(buffer, "[ERROR] person to string buffer alloc");
	snprintf(buffer, BUFFER_SIZE, "%s, age %d", item->name, item->age);
	return buffer;
}

void showcase_list_float() {
	list_float *origin = empty_list(float), *head = origin;

	const float f1 = 3.141, f2 = 2.71, f3 = 9.81;
	head = append_float(head, &f1);
	head = append_float(head, &f2);
	head = append_float(head, &f3);

	printf("List of floats:\n");
	printlst_float(head, float_to_string);
	freelst_float(head);
}

void showcase_list_person() {
	list_person *origin = empty_list(person), *head = origin;                                                     
	const person p1 = { "John", 20 };
	const person p2 = { "Jane", 18 };
	const person p3 = { "William", 31 };
	head = append_person(head, &p1);                            head = append_person(head, &p2);
	head = append_person(head, &p3);

	printf("List of people:\n");
	printlst_person(head, person_to_string);
	freelst_person(head);
}

int main(void) {

	showcase_list_float();
	showcase_list_person();
	
	return EXIT_SUCCESS;
}
