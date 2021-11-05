// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

int dic_size = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char low_word[strlen(word)];
    strcpy(low_word, word);
    for (int i = 0; low_word[i] != '\0'; i++)
    {
         low_word[i] = tolower(low_word[i]);
    }
    int index = hash(low_word);
    if (table[index] != NULL)
    {
        for (node *nodeptr = table[index]; nodeptr != NULL; nodeptr = nodeptr->next)
        {
            if (strcmp(nodeptr->word, low_word) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Source for Hash Function: Deliberate Think
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            n = word[i] - 'a' + 1;
        }
        else
        {
            n = 27;
        }
        hash = ((hash << 3) + n) % N;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dicfile = fopen(dictionary, "r");
    if (dicfile == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    node *new_nodeptr;
    while(fscanf(dicfile, "%s", word) != EOF)
    {
        dic_size++;
        new_nodeptr = malloc(sizeof(node));
        if (new_nodeptr == NULL)
        {
            return false;
        }
        strcpy(new_nodeptr->word, word);
        int index = hash(word);
        if (table[index] == NULL)
        {
            new_nodeptr->next = NULL;
            table[index] = new_nodeptr;
        }
        else
        {
            new_nodeptr->next = table[index];
            table[index] = new_nodeptr;
        }
    }
    fclose(dicfile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dic_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *cursor = table[i];
            while (cursor != NULL)
            {
                node *tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }
    return true;
}
