/*
 * dictionary.h
 *
 *  Created on: Feb 17, 2018
 *      Author: x-wing
 */
#ifndef DICTIONARY_H_
#define DICTIONARY_H_


#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef enum  {ARRAY_KEY, STRING_KEY, PRIMITIVE_KEY, NUM_KEYS} KEY_TYPE;

static struct Node_t
{
	struct Node_t *left;
	struct Node_t *right;
	struct Node_t *down;
	char  key;
	void* data;
} default_Node = {NULL, NULL, NULL,'Z', NULL};
typedef struct Node_t Node;


typedef struct
{
	Node *head;
	Node *tail;
	ulong key_len;
	KEY_TYPE key_type;
} Dictionary;


/* CONSTRUCTORS */
Dictionary array_dict(ulong);
static const Dictionary string_dict  = {NULL, NULL, 0, STRING_KEY};
static const Dictionary default_dict = {NULL, NULL, 1, PRIMITIVE_KEY};


/* FUNCTIONS */
void*  insert_datum_key (Dictionary*, char*, ulong, ulong);
void*  insert_array_key (Dictionary*, char*, ulong, ulong);
void*  insert_string_key(Dictionary*, char*, ulong);
uchar  lookup_datum_key (Dictionary*, char*, ulong);
uchar  lookup_array_key (Dictionary*, char*, ulong);
uchar  lookup_string_key(Dictionary*, char*);
void   destructor(Dictionary*);


/* MACROS */
#define insert(dict, key, data_in) (*(typeof(data_in)*)insert_datum_key(&dict, (char*)&key, sizeof(key), sizeof(data_in)) = data_in)
#define lookup(dict, key) (lookup_datum_key(&dict, (char*)&key, sizeof(key)))
#define insert_key(dict, key, data_in) ((dict.key_type == ARRAY_KEY) ? \
				  (*(typeof(data_in)*)insert_array_key(&dict, (char*)key, sizeof(key[0]), sizeof(data_in)) = data_in) : \
				  (*(typeof(data_in)*)insert_string_key(&dict, key, sizeof(data_in)) = data_in))
#define lookup_key(dict, key) ((dict.key_type == ARRAY_KEY) ? \
				  (lookup_array_key(&dict, (char*)key, sizeof(key[0]))) : \
				  (lookup_string_key(&dict, key)))
#define retreive(dict, data_out) ( data_out = *(typeof(data_out)*)dict.tail->data )


#endif /* DICTIONARY_H_ */
