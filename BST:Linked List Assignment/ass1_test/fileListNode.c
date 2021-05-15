//fileListNode based on week02 DLList.c
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "fileListNode.h"

/** Create a new DLListNode (private function) */
FileList newDLListNode (char *str) {
	FileList new = malloc (sizeof *new);
	int count = 0;
	int countLength = strlen(str);
	char *concreteWord1 = malloc((countLength+1)*sizeof(char));
	while (count < countLength) {
		concreteWord1[count] = str[count];
		count++;
	}
	concreteWord1[count] = '\0';
	new->filename = concreteWord1;
	new->tf = 1;
    new->next = NULL;
	return new;
}

