//BST functions for invertedIndex.c
//based from week 3 file BSTree.c

#include "invertedIndexBST.h"
#include <stddef.h>

// create a new empty invertedIndexBST
InvertedIndexBST newInvertedIndexBST (void) {
	return NULL;
}

// make a new node containing a value
static InvertedIndexBST newInvertedIndexBSTNode (char *str) {
	InvertedIndexBST new = malloc (sizeof *new);
	int count = 0;
	int countLength = strlen(str);
	if (new == NULL) err (EX_OSERR, "couldn't allocate InvertedIndexBST node");
	char *concreteWord1 = malloc((countLength+1)*sizeof(char));
	while (count < countLength) {
		concreteWord1[count] = str[count];
		count++;
	}
	concreteWord1[count] = '\0';
	new->word = concreteWord1;
	new->left = new->right = NULL;
    new->fileList = NULL;
	return new;
}

// insert a new value into an invertedIndexBST
InvertedIndexBST invertedIndexBSTInsert (InvertedIndexBST t, char *str) {
	
	if (t == NULL) {
		return newInvertedIndexBSTNode(str);
    } else if (strcmp(str, t->word) < 0) {
        t->left = invertedIndexBSTInsert(t->left, str);
    } else if (strcmp(str, t->word) > 0) {
        t->right = invertedIndexBSTInsert(t->right, str);
    } 
    return t;
}

// check whether a value is in an invertedIndexBST
int invertedIndexBSTreeFind (InvertedIndexBST t, char *str) {
	if (t == NULL) {
		return 0;
	} else if (strcmp(str, t->word) < 0) {
		return invertedIndexBSTreeFind (t->left, str);
	} else if (strcmp(str, t->word) > 0) {
		return invertedIndexBSTreeFind (t->right, str);
	} else {
		return 1;
	}
}