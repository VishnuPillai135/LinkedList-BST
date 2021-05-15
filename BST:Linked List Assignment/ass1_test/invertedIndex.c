#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <assert.h>
#include <stdlib.h>
#include <sysexits.h>
#include <math.h>
#include "invertedIndexBST.h" 
#include "fileListNode.h"
#include "tfIDF.h"



#define maxArrayLength 9999
void printHelper (InvertedIndexBST tree, FILE *fp);


char *normaliseWord(char *str) {
    int count = 0;
    int countLength = strlen(str);
    
    while (str[0] == ' ') { //delete all leading spaces
        str = &str[1];
        countLength--;
    } 

    while (str[countLength - 1] == ' ') {   //delete all trailing spaces
        str[countLength - 1] = '\0';
        countLength--;
    }

    if ((str[countLength - 1] == '.') || (str[countLength - 1] == ',') ||   //delete 1 of 4 characters at the end
        (str[countLength - 1] == ';') || (str[countLength - 1] == '?')) {
        str[countLength - 1] = '\0';
    }

    while (str[count] != '\0') {    //make lower case
        str[count] = tolower(str[count]);
        count++;
    }
    return str;
}

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    
    FILE* collectionTXT = fopen("collection.txt","r");
    char fileBuffer[maxArrayLength]; 
    char wordBuffer[maxArrayLength];
    char wordBuffer1[maxArrayLength];
    InvertedIndexBST mainTree = newInvertedIndexBST();
    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) { // 1.) scan through collection.txt
        normaliseWord(fileBuffer);
        double countTotal = 0;
        FILE *fileTXT = fopen(fileBuffer, "r");     //2.) open each file
        if (fileTXT == NULL) {  //making sure file exists
            printf("file: %s doesn't exist\n", fileBuffer);
        } else {
            while (fscanf(fileTXT, "%s", wordBuffer) != EOF) {
                normaliseWord(wordBuffer);
                mainTree = invertedIndexBSTInsert(mainTree, wordBuffer);    //3.) insert new node
                InvertedIndexBST findTree = invertedIndexBSTreeFind(mainTree, wordBuffer);  //4.) find node in BST
                FileList new = newDLListNode(fileBuffer);    //5.) create new FileListNode
                //6.) insert fileListNode into BST
                if (findTree->fileList == NULL) {   //if empty
                    findTree->fileList = new;
                    new->next = NULL;
                } else {    //alphabetically insert
                    FileList curr = findTree->fileList;
                    while (curr != NULL) {
                        if (strcmp(new->filename, curr->filename) < 0) {    //if smaller than the first element
                            new->next = curr;
                            findTree->fileList = new;
                            break;
                        }
                        else if (curr->next == NULL) { //if last element
                            if (strcmp(new->filename, curr->filename) != 0) {
                                curr->next = new;
                                break;
                            } else {    //INCREMEMENT FREQUENCY OF WORD
                                curr->tf++;
                            }
                        }
                        else if ((strcmp(new->filename, curr->filename) > 0) && 
                            (strcmp(new->filename, curr->next->filename) < 0)) { //inbetween
                            FileList curr1 = curr->next;
                            curr->next = new;
                            new->next = curr1;
                            break;
                        } else if (strcmp(new->filename, curr->filename) == 0) {
                            curr->tf++;
                        }
                        curr = curr->next;
                    }
                }
                countTotal++;
            }
            // 7.) finding frequency of FileList
            FILE *fileTXT1 = fopen(fileBuffer, "r");
            while (fscanf(fileTXT1, "%s", wordBuffer1) != EOF) {
                normaliseWord(wordBuffer1);
                InvertedIndexBST findTreeFrequency = invertedIndexBSTreeFind(mainTree, wordBuffer1);
                FileList curr2 = findTreeFrequency->fileList;
                while (curr2 != NULL) {
                    if (curr2->tf >= 1) {
                        curr2->tf = curr2->tf/countTotal;
                    }
                    curr2 = curr2->next;
                } 
            }
        }
    }
    return mainTree; 
} 

//main print function
void printInvertedIndex(InvertedIndexBST tree) {
    FILE *fp = fopen("invertedIndex.txt", "w+");
    printHelper(tree, fp);
    fclose(fp);
}

//helper function for print function
void printHelper (InvertedIndexBST tree, FILE *fp) {
    if (tree != NULL) {
        printHelper(tree->left, fp);
        FileList curr = tree->fileList;
        fprintf(fp, "%s ", tree->word);
        while (curr != NULL) {
            fprintf(fp, "%s ", curr->filename);
            curr = curr->next;
        }
        fprintf(fp, "\n");
        printHelper(tree->right, fp);
    }
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {

    InvertedIndexBST mainTree = invertedIndexBSTreeFind(tree, searchWord);

    double count = 0;
    double idf;
    double division;

    // 1.) find total number of documents containing word
    FileList curr = mainTree->fileList;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    // 2.) find idf
    division = D/count;
    idf = log10(division);

    // 3.) create unsorted linked list
    //create first node in list
    curr = mainTree->fileList;
    TfIdfList mainIDF = newtfIDFNode(curr->filename);
    TfIdfList mainCurr = mainIDF;
    double tf = curr->tf;
    mainIDF->tfIdfSum = tf * idf;
    curr = curr->next;

    //create the rest of the nodes
    while (curr != NULL) {
        mainIDF->next = newtfIDFNode(curr->filename);
        mainIDF = mainIDF->next;
        tf = curr->tf;
        mainIDF->tfIdfSum = tf * idf;
        curr = curr->next;
    }

////////////////////////////////////////////////////

    // 4.) sort unsorted linked list in descending tf-idf order
    //create a new sorted list
    //set rep->first
    TfIdfList finalList = newtfIDFNode(mainCurr->filename);
    tfRep rep = newtfRep();
    rep->first = finalList;
    finalList->tfIdfSum = mainCurr->tfIdfSum;
    //move to second item in original list
    mainCurr = mainCurr->next;

    while (mainCurr != NULL) {  //inserting each node from the original list
        finalList = rep->first;
        while (finalList != NULL) { //comparing to each node in the new list
            TfIdfList new = newtfIDFNode(mainCurr->filename);   //create new node for new list
            new->tfIdfSum = mainCurr->tfIdfSum;
            if (mainCurr->tfIdfSum > rep->first->tfIdfSum) {    //if greater than the first element
                new->next = rep->first;
                rep->first = new;
                break;
            }
            if (finalList->tfIdfSum == mainCurr->tfIdfSum) {    //if tfid sum is equal, sort alphabetically //2 CONDITION
                if (strcmp(mainCurr->filename, finalList->filename) > 0) {    
                    TfIdfList temp = finalList->next;
                    finalList->next = new;
                    new->next = temp;
                    break;
                } 
            }
            if (finalList->next == NULL) {  
                finalList->next = new;
                break;
            }
            //if inbetween two numbers
            if ((mainCurr->tfIdfSum < finalList->tfIdfSum) && (mainCurr->tfIdfSum > finalList->next->tfIdfSum)) {
                TfIdfList temp = finalList->next;
                finalList->next = new;
                new->next = temp;
                break;
            }
            finalList = finalList->next;
        }
        mainCurr = mainCurr->next;
        
    }

    finalList = rep->first;
    return finalList;
}

TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[], int D) {

    //1.) place all relevant files into 1 linked list
    TfIdfList mainList = calculateTfIdf(tree, searchWords[0], D);
    tfRep mainRep = newtfRep();
    TfIdfList new = newtfIDFNode(mainList->filename);   //create new node for new list
    new->tfIdfSum = mainList->tfIdfSum;
    mainRep->first = new;
    while (mainList != NULL) {
        new->next = newtfIDFNode(mainList->filename);   //create new node for new list
        new->next->tfIdfSum = mainList->tfIdfSum;
        new = new->next;
        mainList = mainList->next;
    }
    int count = 1;
    while (searchWords[count] != NULL) {
        mainList = calculateTfIdf(tree, searchWords[count], D);
        count++;
        while (mainList != NULL) {
            new->next = newtfIDFNode(mainList->filename);   //create new node for new list
            new->next->tfIdfSum = mainList->tfIdfSum;
            new = new->next;
            mainList = mainList->next;
        }
    }

    // 2.) sort items based on tf-idf descending order; use previous sorting code
    TfIdfList mainCurr = mainRep->first;
    TfIdfList finalList = newtfIDFNode(mainCurr->filename);
    tfRep rep = newtfRep();
    rep->first = finalList;
    finalList->tfIdfSum = mainCurr->tfIdfSum;
    //move to second item in original list
    mainCurr = mainCurr->next;

    while (mainCurr != NULL) {  //inserting each node from the original list
        finalList = rep->first;
        while (finalList != NULL) { //comparing to each node in the new list
            TfIdfList new = newtfIDFNode(mainCurr->filename);   //create new node for new list
            new->tfIdfSum = mainCurr->tfIdfSum;
            if (mainCurr->tfIdfSum > rep->first->tfIdfSum) {    //if greater than the first element
                new->next = rep->first;
                rep->first = new;
                break;
            }
            if (finalList->tfIdfSum == mainCurr->tfIdfSum) {    //if tfid sum is equal, sort alphabetically //2 CONDITION
                if (strcmp(mainCurr->filename, finalList->filename) > 0) {    
                    TfIdfList temp = finalList->next;
                    finalList->next = new;
                    new->next = temp;
                    break;
                } 
            }
            if (finalList->next == NULL) {  
                finalList->next = new;
                break;
            }
            if ((mainCurr->tfIdfSum < finalList->tfIdfSum) && (mainCurr->tfIdfSum > finalList->next->tfIdfSum)) {
                TfIdfList temp = finalList->next;
                finalList->next = new;
                new->next = temp;
                break;
            }
            finalList = finalList->next;
        }
        mainCurr = mainCurr->next;
        
    }

    return finalList;
}

