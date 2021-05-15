//BST functions for invertedIndex.c
//based from week 3 file BSTree.c

#include "invertedIndex.h"

// create a new empty invertedIndexBST
InvertedIndexBST newInvertedIndexBST (void);

// make a new node containing a value
static InvertedIndexBST newInvertedIndexBSTNode (char *str);

// insert a new value into an invertedIndexBST
InvertedIndexBST invertedIndexBSTInsert (InvertedIndexBST t, char *str);

// check whether a value is in an invertedIndexBST
int invertedIndexBSTreeFind (InvertedIndexBST t, char *str);