/*
 * dictionary.h
 *
 *  Created on: Feb 17, 2018
 *      Author: x-wing
 */
#ifndef DICTIONARY_H_
#define DICTIONARY_H_


#include <stdlib.h>
#include <stdarg.h>


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;


static struct Node_t
{
	char  key;
	void* data;

	struct Node_t *down;
	struct Node_t *left;
	struct Node_t *right;
} default_Node = {'Z', NULL, NULL, NULL, NULL};
typedef struct Node_t Node;


static struct Dictionary_t
{
	Node *head;
	Node *tail;
} default_dict = {NULL, NULL};
typedef struct Dictionary_t Dictionary;


Dictionary new_Dictionary();
void*  insert_key(Dictionary *dict, char *key, ulong key_size, ulong data_size);
uchar  lookup_key(Dictionary *dict, char *key, ulong key_size);
void   destructor(Dictionary *d);

#define insert(dict, key, data_in) (*(typeof(data_in)*)insert_key(&dict, (char*)&key, sizeof(key), sizeof(data_in)) = data_in)
#define lookup(dict, key) (lookup_key(&dict, (char*)&key, sizeof(key)))
#define retreive(dict, data_out) ( data_out = *(typeof(data_out)*)dict.tail->data )


#endif /* DICTIONARY_H_ */
