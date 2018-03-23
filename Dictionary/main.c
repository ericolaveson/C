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
	int  same_key = 11;
	int  different_key;

	int *key1 = &same_key;
	int *key2 = &same_key;
	int *key3 = &same_key;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int test1 = 0;
	int test2 = 0;
	int test3 = 0;

	insert(dict, key1, data1);

	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);

	same_key = 22;

	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data1);

	same_key = 33;

	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data1);

	key1 = &different_key;

	/* reset test values */
	test2 = 0;
	test3 = 0;

	assert(!lookup(dict, key1));
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data1);
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data1);

	insert(dict, key1, data2);

	/* reset test values */
	test1 = 0;
	test2 = 0;
	test3 = 0;

	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data2);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data1);
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data1);

	key3 = key1;

	/* reset test values */
	test1 = 0;
	test2 = 0;
	test3 = 0;

	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data2);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data1);
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data2);

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

	int test1;
	int test2;
	int test3;

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(!lookup(dict, key1));
	assert(!lookup(dict, key2));
	assert(!lookup(dict, key3));

	insert(dict, key1, data1);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(!lookup(dict, key2));
	assert(!lookup(dict, key3));

	insert(dict, key2, data2);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data2);
	assert(!lookup(dict, key3));

	insert(dict, key3, data3);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data2);
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data3);

	destructor(&dict);
	printf("PASSED: test_key_type_varieties\n");
}


void test_data_type_varieties()
{
	Dictionary dict = default_dict;
	double key1 = 111.111;
	double key2 = 222.222;
	double key3 = 333.333;

	char   data1 = 'a';
	int    data2 = 77;
	double data3 = 8888.8888;

	char   test1 = 'F';
	int    test2 = 33;
	double test3 = 555.5555;

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(!lookup(dict, key1));
	assert(!lookup(dict, key2));
	assert(!lookup(dict, key3));

	insert(dict, key1, data1);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(!lookup(dict, key2));
	assert(!lookup(dict, key3));

	insert(dict, key2, data2);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data2);
	assert(!lookup(dict, key3));

	insert(dict, key3, data3);

	/* VERIFY ALL CONTENTS OF DICTIONARY */
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) == data1);
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) == data2);
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) == data3);

	destructor(&dict);
	printf("PASSED: test_data_type_varieties\n");
}


void test_key_and_data_type_varieties_together()
{
	Dictionary dict = default_dict;
	char   key1 = 'a';
	int    key2 = 444;
	double key3 = 111.111;

	char   data1 = 'a';
	int    data2 = 77;
	double data3 = 8888.8888;

	insert(dict, key1, data1);
	insert(dict, key2, data2);
	insert(dict, key3, data3);

	assert(contains_pair(dict, key1, data1));

	char fraud = 'Z';
	char test1;
	assert(!lookup(dict, fraud));
	assert(lookup(dict, key1));
	assert(retreive(dict, test1) ==  data1);

	int test2;
	assert(lookup(dict, key2));
	assert(retreive(dict, test2) ==  data2);

	double test3;
	assert(lookup(dict, key3));
	assert(retreive(dict, test3) ==  data3);

	destructor(&dict);
	printf("PASSED: test_key_and_data_type_varieties_together\n");
}


void test_array_key()
{
	const int KEY_LENGTH = 10;

	Dictionary dict = array_dict(KEY_LENGTH);
	int  i;
	int  data = 22;
	int  test = 33;
	char key[KEY_LENGTH];
	for (i = 0; i < KEY_LENGTH; ++i)
		key[i] = 'a';

	insert_key(dict, key, data);

	assert(lookup_key(dict, "aaaaaaaaaa"));
	assert(retreive(dict, test) == data);

	/* change edges to test off by one */
	assert(!lookup_key(dict, "aaaaaaaaaA"));
	assert(!lookup_key(dict, "Aaaaaaaaaa"));

	destructor(&dict);
	printf("PASSED: test_array_key\n");
}


void test_int_array_key()
{
	const int KEY_LENGTH = 7;
	Dictionary dict = array_dict(KEY_LENGTH);

	int i;
	char data = 'z';
	char test = 'F';
	int  key1[] = {0,2,2,7,4,5,5};
	int *key2   = (int*) malloc(sizeof(int)*KEY_LENGTH);

	for (i = 0; i < KEY_LENGTH; ++i)
		key2[i] = key1[i];

	insert_key(dict, key1, data);
	assert(lookup_key(dict, key2));
	assert(retreive(dict, test) == data);

	/* change last index to test off by one */
	int test_key1[] = {0,2,2,7,3,5,5};
	assert(!lookup_key(dict, test_key1));

	int test_key2[] = {0,2,2,7,4,5,6};
	assert(!lookup_key(dict, test_key2));

	key2[KEY_LENGTH-1] = 6;
	insert_key(dict, key2, data);
	assert(lookup_key(dict, key2));
	assert(retreive(dict, test) == data);

	assert(!lookup_key(dict, test_key1));
	assert(lookup_key(dict, test_key2));

	free(key2);
	destructor(&dict);
	printf("PASSED: test_int_array_key\n");
}


void test_string_key()
{
	Dictionary dict = string_dict;
	uchar data1 = 0xff;
	uchar data2 = 0x16;
	uchar test  = 0;

	insert_key(dict, "key1", data1);
	assert(lookup_key(dict, "key1"));
	assert(retreive(dict, test) == data1);

	insert_key(dict, "key2", data2);
	assert(lookup_key(dict, "key2"));
	assert(retreive(dict, test) == data2);

	assert(!lookup_key(dict, "key12"));
	assert(!lookup_key(dict, "key22"));
	assert(!lookup_key(dict, "key"));
	assert(!lookup_key(dict, "ke"));
	assert(!lookup_key(dict, "k"));
	assert(!lookup_key(dict, ""));

	destructor(&dict);
	printf("PASSED: test_string_key\n");
}


void test_multiple_inserts()
{
	const int BOUND = 10;

	Dictionary dict = default_dict;
	int  i;
	int  key  = 99;
	char data1 = 'Z';
	char data2 = 'Y';
	char test  = 'F';

	insert(dict, key, data1);
	assert(lookup(dict, key));
	assert(retreive(dict, test) == data1);

	test = 'F';
	insert(dict, key, data1);
	assert(lookup(dict, key));
	assert(retreive(dict, test) == data1);

	for (i = 0; i < BOUND; ++i)
		insert(dict, key, data1);

	test = 'F';
	assert(lookup(dict, key));
	assert(retreive(dict, test) == data1);

	insert(dict, key, data2);

	test = 'F';
	assert(lookup(dict, key));
	assert(retreive(dict, test) != data1);
	assert(test == data2);

	destructor(&dict);
	printf("PASSED: test_multiple_inserts\n");
}


void test_multiple_same_type_dictionaries()
{
	Dictionary positive = default_dict;
	Dictionary negative = default_dict;

	int key1 = 1;
	int key2 = 2;
	int key3 = 3;

	int test1 = 0;
	int test2 = 0;
	int test3 = 0;

	insert(positive, key1, key1);
	assert(!lookup(negative, key1));
	insert(negative, key1, -key1);

	assert(lookup(positive, key1));
	assert(retreive(positive, test1) == key1);

	assert(lookup(negative, key1));
	assert(retreive(negative, test1) == -key1);

	destructor(&positive);
	destructor(&negative);
	printf("PASSED: test_multiple_same_type_dictionaries\n");
}


void test_multiple_different_type_dictionaries()
{
	Dictionary def_dict = default_dict;
	Dictionary str_dict = string_dict;
	Dictionary arr_dict = array_dict(10);

	uchar def_key = 0xff;
	char *str_key = "key";
	int   arr_key[] = {8,8,8,8,8,8,8,8,8,8};

	char   def_data = -1;
	int    str_data = 100;
	double arr_data = 3.333;

	char   def_test = 0;
	int    str_test = 0;
	double arr_test = 0.0;

	insert(def_dict, def_key, def_data);
	assert(lookup(def_dict, def_key));
	assert(retreive(def_dict, def_test) == def_data);

	insert_key(str_dict, str_key, str_data);
	assert(lookup_key(str_dict, str_key));
	assert(retreive(str_dict, str_test) == str_data);

	insert_key(arr_dict, arr_key, arr_data);
	assert(lookup_key(arr_dict, arr_key));
	assert(retreive(arr_dict, arr_test) == arr_data);

	def_test = 0;
	str_test = 0;
	arr_test = 0.0;

	assert(lookup(def_dict, def_key));
	assert(retreive(def_dict, def_test) == def_data);

	assert(lookup_key(str_dict, str_key));
	assert(retreive(str_dict, str_test) == str_data);

	assert(lookup_key(arr_dict, arr_key));
	assert(retreive(arr_dict, arr_test) == arr_data);

	destructor(&def_dict);
	destructor(&str_dict);
	destructor(&arr_dict);
	printf("PASSED: test_multiple_different_type_dictionaries\n");
}


int main()
{
	srand(time(NULL));

	test_strictly_increasing_keys();
	test_strictly_decreasing_keys();
	test_unique_random_keys();
	test_random_keys();
	test_double_key();
	test_double_data();
	test_primitive_data();
	test_pointer_data();
	test_overwrite_data();
	test_pointer_keys();
	test_pointer_key_overwrite();
	test_key_type_varieties();
	test_data_type_varieties();
	test_key_and_data_type_varieties_together();
	test_array_key();
	test_int_array_key();
	test_string_key();

	test_multiple_inserts();
	test_multiple_same_type_dictionaries();
	test_multiple_different_type_dictionaries();

	printf("\nProgram has terminated gracefully\n");

	return 0;
}
