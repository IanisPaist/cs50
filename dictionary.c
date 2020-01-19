// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //make all chars lower
    char word_lower[100];

    //clear word_lower
    for (int i = 0; i < 100; i++)
    {
        word_lower[i] = 0;
    }

    //copy word into word_lower
    for (int i = 0; i < strlen(word); i++)
    {
        word_lower[i] = tolower(word[i]);
    }

    //hash the word and get an index for hash table
    int n = hash(word_lower);

    //current is a pointer to a node - first element
    node* current = table[n];

    while (current != NULL)
    {
        int compare_result  = strcasecmp(current->word,word_lower);

        if (compare_result == 0)
        {
            return true;
        }
        else
        {
            current = current->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    //Copyright (c) 2005-2008, Simon Howard
	//https://github.com/fragglet/c-algorithms/blob/master/src/hash-string.c
	/* This is the djb2 string hash function */

	unsigned int result = 5381;
	unsigned char *p;

	p = (unsigned char *) word;

	while (*p != '\0')
	{
		result = ((result << 5) + result + *p) % N;
		++p;
	}

	return result;
}


//for counting words
unsigned int number_of_words = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    FILE *dict = fopen(dictionary,"r");
    if (dict == NULL)
    {
        printf("Coundn't open the dictionary file\n");
        return false;
    }

    //READ STRINGS FROM FILE
    //create array to hold string
    char current_word[LENGTH + 1];

    //loop until EOF
    while (fscanf(dict, "%s", current_word) != EOF)
    {

        //allocate memory to the pointer to new node
        node* new_node = malloc(sizeof(node)); //pointer
        if (new_node == NULL)
        {
            printf("Coundn't allocate memory\n");
            return false;
        }

        //copy word into new node
        strcpy(new_node->word, current_word);
        new_node->next = NULL;

        //hash this word
        unsigned int n = hash(current_word);

        //head is a pointer to a first node
        node* head = table[n];

        //new_node is a pointer to new node. Set it's "next" value equal to pointer to first element
        new_node->next = head;

        //set pointer in hash table array to new_node, making it new first element
        table[n] = new_node;

        //count words;
        number_of_words++;

        //clear current_word
        for (int i = 0; i < LENGTH + 1; i++)
        {
            current_word[i] = 0;
        }
    }

    fclose(dict);
    //free(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return number_of_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    //iterate over each element of hash table
    for(int i = 0; i < N; i++)
    {
        //create two pointers to keep track of elements
        node* tmp = table[i];
        node* cursor = table[i];

        //go to end of linked list and free nodes
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
