/*
 * dictionary.c
 *
 *  Created on: Feb 17, 2018
 *      Author: x-wing
 */
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
 * Create a new dictionary indexed by array keys.
 */
Dictionary array_dict(ulong len)
{
	Dictionary dict;
	dict.head = NULL;
	dict.tail = NULL;
	dict.key_len = len;
	dict.key_type = ARRAY_KEY;
	return dict;
}


/**
 * FUNCTION
 *
 * Traverses the dictionary, paving a path of key bytes along the way.
 */
void* insert_datum_key(Dictionary *dict, char *key, ulong key_size, ulong data_size)
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
 * Insert data into a dictionary by array key.
 */
void* insert_array_key(Dictionary *dict, char *key, ulong key_size, ulong data_size)
{
	ulong len = dict->key_len * key_size;
	return insert_datum_key(dict, key, len, data_size);
}


/**
 * FUNCTION
 *
 * Insert data into a dictionary by null terminating string key.
 */
void* insert_string_key(Dictionary *dict, char *key, ulong data_size)
{
	if (key == NULL || !key[0])
		return NULL;

	int i = 0;
	if (dict->head == NULL)
		dict->head = new_Node(key[0]);

	Node *node = dict->head;

	while(key[i])
	{
		if (key[i] < node->key)
		{
			if (node->left == NULL)
				node->left = new_Node(key[i]);
			node = node->left;
		}
		else if (key[i] > node->key)
		{
			if (node->right == NULL)
				node->right = new_Node(key[i]);
			node = node->right;
		}
		else if (key[++i])
		{
			if (node->down == NULL)
				node->down = new_Node(key[i]);
			node = node->down;
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
uchar lookup_datum_key(Dictionary *dict, char *key, ulong key_size)
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
 * Index into a dictionary by array key.
 */
uchar lookup_array_key(Dictionary *dict, char *key, ulong key_size)
{
	ulong len = dict->key_len * key_size;
	return lookup_datum_key(dict, key, len);
}


/**
 * FUNCTION
 *
 * Index into a dictionary by string key.
 */
uchar lookup_string_key(Dictionary *dict, char *key)
{
	dict->tail = NULL;

	if  (key == NULL || !key[0])
		return 0;

	Node *node = dict->head;
	int i = 0;

	while (node && key[i])
	{
		if (key[i] < node->key)
			node = node->left;
		else if  (key[i] > node->key)
			node = node->right;
		else if (key[++i])
			node = node->down;
	}

	if (node && node->key == key[i-1] && node->data)
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
