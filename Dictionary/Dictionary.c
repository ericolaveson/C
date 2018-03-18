/*
 * Dictionary.c
 *
 *  Created on: Feb 17, 2018
 *      Author: x-wing
 */
#include <stdio.h>
#include <string.h>
#include "Dictionary.h"


/**
 * FUNCTION
 *
 * Create a new node for the dictionary.
 */
Node* new_Node(char key)
{
	Node* node  = (Node*) malloc(sizeof(Node));
	*node = default_Node;
	node->key   = key;
	return node;
}


/**
 * FUNCTION
 *
 * Traverses the dictionary, paving a path of key bytes along the way.
 */
void* insert_key(Dictionary *dict, char *key, ulong key_size, ulong data_size)
{
	if (key == NULL)
		return NULL;
	if (dict->head == NULL)
		dict->head = new_Node(key[0]);

	Node *node = dict->head;
	int i = 0;

	while(i < key_size - 1)
	{
		if (key[i] == node->key)
		{
			++i;
			if (node->down == NULL)
				node->down = new_Node(key[i]);
			node = node->down;
		}
		else if (key[i] < node->key)
		{
			if (node->left == NULL)
				node->left = new_Node(key[i]);
			node = node->left;
		}
		else
		{
			if (node->right == NULL)
				node->right = new_Node(key[i]);
			node = node->right;
		}
	}

	if (node->data != NULL)
		free(node->data);
	node->data = (void*) malloc(data_size);
	return node->data;
}


/**
 * FUNCTION
 *
 * Traverses the dictionary for data paired with the corresponding key.
 */
uchar lookup_key(Dictionary *dict, char *key, ulong key_size)
{
	if  (key == NULL)
	{
		dict->tail = NULL;
		return 0;
	}

	Node *node = dict->head;
	int i = 0;

	while (node && i < key_size - 1)
	{
		if (key[i] == node->key)
		{
			node = node->down;
			++i;
		}
		else if (key[i] < node->key)
			node = node->left;
		else
			node = node->right;
	}

	if (node && node->key == key[i])
	{
		dict->tail = node;
		return 1;
	}
	else
	{
		dict->tail = NULL;
		return 0;
	}
}


/**
 * FUNCTION
 *
 * De-allocate all memory for a single dictionary node.
 */
void destroy_dictionary_tree(Node *node)
{
	if (node)
	{
		destroy_dictionary_tree(node->left);
		destroy_dictionary_tree(node->right);
		destroy_dictionary_tree(node->down);

		if (node->data)
			free(node->data);
		free(node);
	}
}


/**
 * FUNCTION
 *
 * Destroy a node and all of its children nodes.
 */
void destructor(Dictionary *dict)
{
	if (dict->head)
		destroy_dictionary_tree(dict->head);
}
