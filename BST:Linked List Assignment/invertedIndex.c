#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <assert.h>
#include <stdlib.h>
#include <sysexits.h>
#include "invertedIndexBST.c" // <-- #include invertedIndexBST.c <-- invertedIndexBST.h <--- invertedIndex.h

#define maxArrayLength 9999

char *normaliseWord(char *str);
int generateInvertedIndex1 (char *collectionFilename);
InvertedIndexBST generateInvertedIndex(char *collectionFilename);
void printInvertedIndex(InvertedIndexBST tree); 

int main (void) {

    //TESTING WHETHER NORMALISEWORD WORKS
    char testing[] = "collection.txt";
    normaliseWord(testing);
    generateInvertedIndex1(testing);
    return 0;
}


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
        // printf("%c", str[count]);    //testing to print out final normalised word
        count++;
    }
    return str;
}

int generateInvertedIndex1 (char *collectionFilename) {
    

    FILE* collectionTXT = fopen("collection.txt","r");
    char fileBuffer[maxArrayLength]; 
    char wordBuffer[maxArrayLength];
    // char concreteWord[maxArrayLength];
    // int count;
    // int countLength;
    InvertedIndexBST mainTree = newInvertedIndexBST();
    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) { // 1.) scan through collection.txt
        normaliseWord(fileBuffer);
        FILE *fileTXT = fopen(fileBuffer, "r");     //2.) open each file
        if (fileTXT == NULL) {  //making sure file exists
            printf("file: %s doesn't exist\n", fileBuffer);
        } else {
            while (fscanf(fileTXT, "%s", wordBuffer) != EOF) {
                mainTree = invertedIndexBSTInsert(mainTree, wordBuffer);    //3.) insert new node
                printf("the root word is: %s\n", mainTree->word);
            }   
            printf("%s\n", fileBuffer);
        }
    }
    printf("the root word is: %s\n", mainTree->word);
    printf("the word to the left of the root is: %s\n", mainTree->left->word);
    printf("the word to the right of the root is: %s\n", mainTree->right->word);
    printf("the word to the right of the root is: %s\n", mainTree->right->right->word);
    printf("the word to the right of the root is: %s\n", mainTree->right->right->left->word);
    printf("the word to the right of the root is: %s\n", mainTree->right->right->left->left->word);
    printf("the word to the right of the root is: %s\n", mainTree->right->right->left->left->right->word);
    // printf("the word to the right of the root is: %s\n", mainTree->right->right->right->word);

    return 0; 
} 


