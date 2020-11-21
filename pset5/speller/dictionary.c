// Implements a dictionary's functionality

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 48000; //arbitrary num to get the average search req to be 3

// Hash table
node *table[N];

//Counts number of words in dictionary for size function
unsigned int dictWordCount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hashIndex = hash(word);
    node *cursor = table[hashIndex];
    int checking = 1;
    while (checking == 1)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            checking = 0;
            return true;
        }
        else if (cursor->next == NULL)
        {
            checking = 0;
            return false;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) //djb2 algorithm, credits: http://www.cse.yorku.ca/~oz/hash.html
{
    unsigned long hash = 5381;
    int c = 0;

    while (c == *word++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *fileDict = fopen(dictionary, "r");
    if (fileDict == NULL)
    {
        return false;
    }

    int loading = 1; //flag for loading words from dict
    char dictWord[LENGTH + 1]; //buffer for word to be loaded in
    while (loading == 1)
    {
        if (fscanf(fileDict, "%s", dictWord) == EOF) //loads in a word into buffer and check for error
        {
            //end of file
            loading = 0;
            fclose(fileDict);
            return true;
        }

        node *n = malloc(sizeof(node)); //allocate memory for new node
        if (n == NULL)
        {
            loading = 0;
            fclose(fileDict);
            return false;
        }
        strcpy(n->word, dictWord); //assigns word in buffer to new node
        n->next = NULL; //set next pointer to null

        int hashIndex = hash(dictWord);

        if (table[hashIndex] == NULL)
        {
            table[hashIndex] = n;
        }
        else
        {
            n->next = table[hashIndex];
            table[hashIndex] = n;
        }
        dictWordCount++;
    }

    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictWordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *cursor = table[i];
            node *tmp = NULL;
            int unloading = 1;
            while (unloading == 1)
            {
                if (cursor == NULL)
                {
                    unloading = 0;
                    break;
                }
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }
    return true;
}
