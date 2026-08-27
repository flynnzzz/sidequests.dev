/*
 * list.h 
 *
 * Generic linked list implementation/mock with
 * preprocessor directives.
 */
#ifndef LIST_H
#define LIST_H

#define QALLOC(type, n) \
	(type*) malloc(n * sizeof(type))
#define CHECK_ALLOC(ptr, msg) \
	if (ptr == NULL) { \
		perror(msg); \
		exit(1); \
	} 

/*
 * Define flag to allow compilation
 */
#ifdef LIST_TYPE
#include <stdlib.h>
#define DEFINE_LIST(T) \
	typedef struct list_##T##_head { \
		T *data; \
		struct list_##T##_head *next; \
	} list_##T; \
	\
	list_##T *append_##T(list_##T* list, const T* item) { \
		list_##T *list_head = QALLOC(list_##T, 1); \
		CHECK_ALLOC(list_head, "[ERROR] list head allocation"); \
		list_head->data = QALLOC(T, 1); \
		CHECK_ALLOC(list_head->data, "[ERROR] list data allocation"); \
		memcpy(list_head->data, item, sizeof(T)); \
		list_head->next = list; \
		return list_head; \
	} \
	\
	void printlst_##T(list_##T *list, const char* (*to_string)(const T *item)) { \
		if (list != NULL) { \
			printlst_##T(list->next, to_string); \
			const char* str = to_string(list->data); \
			printf("%s\n", str); \
			free((void *)str); \
		} \
		return; \
	} \
	\
	void freelst_##T(list_##T *list) { \
		list_##T* head = list; \
		while (head != NULL) { \
			free((void *)(head->data)); \
			head = head->next; \
		} \
		free((void *)list); \
	}

#define empty_list(T) (list_##T *) NULL

#endif // LIST_TYPE
#endif // LIST_H
