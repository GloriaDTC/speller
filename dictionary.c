// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 45

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char lower_word[LENGTH + 1];
    for (int i = 0; word[i] != '\0'; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[strlen(word)] = '\0';

    unsigned int index = hash(lower_word);
    node *curosr = table[index];

    while (curosr != NULL)
    {
        if (strcmp(curosr->word, lower_word) == 0)
        {
            return true;
        }
        curosr = curosr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    while (*word)
    {
        hash = (hash << 2) ^ tolower(*word++);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    // Read each word in the file
    while (fscanf(source, "%s", word) != EOF)
    {
        // Add each word to the hash table
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        else
        {
            strcpy(n->word, word);
            unsigned int index = hash(word);

            n->next = table[index];
            table[index] = n;
        }
    }

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            count++;
            cursor = cursor->next;
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *curosr = table[i];
        while (curosr != NULL)
        {
            node *tmp = curosr;
            curosr = curosr->next;
            free(tmp);
        }
    }
    return true;
}
