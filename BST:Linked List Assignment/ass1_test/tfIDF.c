#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "fileListNode.h"
#include "tfIDF.h"


TfIdfList newtfIDFNode (char *str) {
	TfIdfList new = malloc (sizeof *new);
	int count = 0;
	int countLength = strlen(str);
	char *concreteWord1 = malloc((countLength+1)*sizeof(char));
	while (count < countLength) {
		concreteWord1[count] = str[count];
		count++;
	}
	concreteWord1[count] = '\0';
	new->filename = concreteWord1;
	new->tfIdfSum = 0;
    new->next = NULL;
	return new;
}

tfRep newtfRep (void) {
	tfRep new = malloc (sizeof *new);
	// if (new == NULL) err (EX_OSERR, "couldn't allocate DLList");
	new->first = NULL;
	return new;
}