#include "invertedIndex.h"

struct tfListRep {
	TfIdfList first;  /**< first node in list */
}; typedef struct tfListRep *tfRep;

TfIdfList newtfIDFNode (char *str);

tfRep newtfRep (void);