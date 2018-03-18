/*
 * main.c
 *
 *  Created on: Feb 17, 2018
 *      Author: x-wing
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Dictionary.h"


static const int BOUND = 1000;
static int test_count  = 0;


#define contains_pair(dict, key, data) ({ \
	typeof(data) found; \
	(lookup(dict, key)) && (retreive(dict, found) == data); })


void assert(uchar expression)
{
	++test_count;
	if (!expression)
	{
		printf("FAILED -- Test #%d\n", test_count);
		exit(1);
	}
}


void test_strictly_increasing_keys()
{
	Dictionary dict = default_dict;
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = i;

	for (i = 0; i < BOUND; ++i)
		insert(dict, keys[i], data[i]);

	for (i = 0; i < BOUND; ++i)
		assert(contains_pair(dict, keys[i], data[i]));

	destructor(&dict);
	printf("PASSED: test_strictly_increasing_keys\n");
}


void test_strictly_decreasing_keys()
{
	Dictionary dict = default_dict;
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = BOUND - i - 1;

	for (i = 0; i < BOUND; ++i)
		insert(dict, keys[i], data[i]);

	for (i = 0; i < BOUND; ++i)
		assert(contains_pair(dict, keys[i], data[i]));

	destructor(&dict);
	printf("PASSED: test_strictly_decreasing_keys\n");
}


void test_unique_random_keys()
{
	Dictionary dict = default_dict;
	int knuth[BOUND];
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
	{
		keys[i] = data[i] = i;
		knuth[i] = rand() % BOUND;
	}

	for (i = 0; i < BOUND; ++i)
	{
		int temp = keys[i];
		keys[i] = keys[knuth[i]];
		keys[knuth[i]] = temp;

		temp = data[i];
		data[i] = data[knuth[i]];
		data[knuth[i]] = temp;
	}

	for (i = 0; i < BOUND; ++i)
		insert(dict, keys[i], data[i]);

	for (i = 0; i < BOUND; ++i)
		assert(contains_pair(dict, keys[i], data[i]));

	destructor(&dict);
	printf("PASSED: test_unique_random_keys\n");
}


void test_random_keys()
{
	Dictionary dict = default_dict;
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = rand() % BOUND;

	for (i = 0; i < BOUND; ++i)
		insert(dict, keys[i], data[i]);

	for (i = 0; i < BOUND; ++i)
		assert(contains_pair(dict, keys[i], data[i]));

	destructor(&dict);
	printf("PASSED: test_random_keys\n");
}


void test_double_key()
{
	Dictionary dict = default_dict;
	double key1  = 5.55555678;
	double key2  = 10.11111144;
	int    data1 = 11;
	int    data2 = 22;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	assert(contains_pair(dict, key2, data2));

	destructor(&dict);
	printf("PASSED: test_double_key\n");
}


void test_double_data()
{
	Dictionary dict = default_dict;
	int    key1  = 11;
	int    key2  = 22;
	double data1 = 5.55555678;
	double data2 = 10.11111144;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	assert(contains_pair(dict, key2, data2));

	destructor(&dict);
	printf("PASSED: test_double_data\n");
}


void test_primitive_data()
{
	Dictionary dict = default_dict;
	int    key1  = 11;
	int    key2  = 22;
	double data1 = 5.55555678;
	double data2 = 10.11111144;
	double data1_initial = data1;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	data1 = 7.77777777;
	assert(!contains_pair(dict, key1, data1));
	assert(contains_pair(dict, key1, data1_initial));

	destructor(&dict);
	printf("PASSED: test_primitive_data\n");
}


void test_pointer_data()
{
	Dictionary dict = default_dict;
	int     key1  = 11;
	int     key2  = 22;
	double  d1    = 5.55555678;
	double  d2    = 10.11111144;
	double *data1 = &d1;
	double *data2 = &d2;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	*data1 = 7.77777777;
	assert(contains_pair(dict, key1, data1));

	destructor(&dict);
	printf("PASSED: test_pointer_data\n");
}


void test_overwrite_data()
{
	Dictionary dict = default_dict;
	int    key   = 11;
	ulong  d1    = 60;
	ulong  d2    = 99;
	ulong *data1 = &d1;
	ulong *data2 = &d2;
	ulong *found;

	insert(dict, key, data1);
	insert(dict, key, data2);

	assert(lookup(dict, key));
	retreive(dict, found);
	assert(found == data2);
	assert(*found == *data2);

	destructor(&dict);
	printf("PASSED: test_overwrite_data\n");
}


void test_pointer_keys()
{
	Dictionary dict = default_dict;
	int  k1    = 77;
	int  k2    = 88;
	int *key1  = &k1;
	int *key2  = &k2;
	int  data1 = 10;
	int  data2 = 20;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	assert(contains_pair(dict, key2, data2));

	key2 = key1;
	assert(contains_pair(dict, key2, data1));

	destructor(&dict);
	printf("PASSED: test_pointer_keys\n");
}


void test_pointer_key_overwrite()
{
	Dictionary dict = default_dict;
	int  k     = 77;
	int *key1  = &k;
	int *key2  = &k;
	int  data1 = 10;
	int  data2 = 20;

	insert(dict, key1, data1);
	insert(dict, key2, data2);

	assert(contains_pair(dict, key1, data2));
	assert(contains_pair(dict, key2, data2));

	destructor(&dict);
	printf("PASSED: test_pointer_key_overwrite\n");
}


void test_key_type_varieties()
{
	Dictionary dict = default_dict;
	char   key1 = 'a';
	int    key2 = 444;
	double key3 = 111.111;

	int data1 = 1;
	int data2 = 2;
	int data3 = 3;

	insert(dict, key1, data1);
	insert(dict, key2, data2);
	insert(dict, key3, data3);

	assert(contains_pair(dict, key1, data1));
	assert(contains_pair(dict, key2, data2));
	assert(contains_pair(dict, key3, data3));

	destructor(&dict);
	printf("PASSED: test_key_type_varieties\n");
}


void test_data_type_varieties()
{

}


void test_key_and_data_type_varieties_together()
{

}


int main()
{
	srand(time(NULL));

//	test_strictly_increasing_keys();
//	test_strictly_decreasing_keys();
//	test_unique_random_keys();
//	test_random_keys();
//	test_double_key();
//	test_double_data();
//	test_primitive_data();
//	test_pointer_data();
//	test_overwrite_data();
//	test_pointer_keys();
//	test_pointer_key_overwrite();


	test_key_type_varieties();

	printf("\nProgram has terminated gracefully\n");

	return 0;
}
