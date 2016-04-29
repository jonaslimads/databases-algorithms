#include <stdio.h>
#include <stdlib.h>

typedef enum { string, identifier, integer } terminal_type;

typedef struct list {
	terminal_type type;

	struct list *next;

	union {
		int i;
		char *string;
		char *identifier;
	};
} list;

int main() {
	list *l;
	l = malloc(sizeof(list));
	l->i = 10;

	list *nl = malloc(sizeof(list));
	nl->i = 20;
	nl->next = NULL;

	l->next = nl;

	list *ptr = l;
	while (ptr != NULL) {
		printf("%d\n", ptr->i);
		ptr = ptr->next;
	}
	return 0;
}