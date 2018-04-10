/*
 * main.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: x-wing
 */

#include "Dictionary.h"

#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <cstring>
#include <stdio.h>

using namespace std;

static const int  BOUND = 1000;
static const char DEFAULT_CHAR = 0xff;
static const int  DEFAULT_INT  = 0;

static double* DEFAULT_DOUBLE_POINTER = NULL;
static ulong*  DEFAULT_ULONG_POINTER = NULL;
static int     assertions_passed = 0;

typedef unsigned char uchar;

void assert(uchar expression)
{
	if (!expression)
	{
		std::cout << "<~~ ASSERT FAILED -- Test # " << assertions_passed;
		exit(1);
	}
	++assertions_passed;
}


void test_strictly_increasing_keys()
{
	Dictionary<int,int> dict(DEFAULT_INT);
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = i;

	for (i = 0; i < BOUND; ++i)
		dict[keys[i]] = data[i];

	for (i = 0; i < BOUND; ++i)
		assert(dict[keys[i]] == data[i]);

	printf("PASSED: test_strictly_increasing_keys\n");
}


void test_strictly_decreasing_keys()
{
	Dictionary<int,int> dict(DEFAULT_INT);
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = BOUND - i - 1;

	for (i = 0; i < BOUND; ++i)
		dict[keys[i]] = data[i];


	for (i = 0; i < BOUND; ++i)
		assert(dict[keys[i]] == data[i]);

	printf("PASSED: test_strictly_decreasing_keys\n");
}


void test_unique_random_keys()
{
	Dictionary<int,int> dict(DEFAULT_INT);
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
		dict[keys[i]] = data[i];

	for (i = 0; i < BOUND; ++i)
		assert(dict[keys[i]] == data[i]);

	printf("PASSED: test_unique_random_keys\n");
}


void test_random_keys()
{
	Dictionary<int,int> dict(DEFAULT_INT);
	int keys[BOUND];
	int data[BOUND];
	int i;

	for (i = 0; i < BOUND; ++i)
		keys[i] = data[i] = rand() % BOUND;

	for (i = 0; i < BOUND; ++i)
		dict[keys[i]] = data[i];

	for (i = 0; i < BOUND; ++i)
		assert(dict[keys[i]] == data[i]);

	printf("PASSED: test_random_keys\n");
}


void test_double_key()
{
	Dictionary<double,int> dict(DEFAULT_INT);
	double key1  = 5.55555678;
	double key2  = 10.11111144;
	int    data1 = 11;
	int    data2 = 22;

	dict[key1] = data1;
	dict[key2] = data2;

	assert(dict[key2] == data2);

	printf("PASSED: test_double_key\n");
}


void test_double_data()
{
	Dictionary<int,double> dict(DEFAULT_INT);
	int    key1  = 11;
	int    key2  = 22;
	double data1 = 5.55555678;
	double data2 = 10.11111144;

	dict[key1] = data1;
	dict[key2] = data2;

	assert(dict[key2] == data2);

	printf("PASSED: test_double_data\n");
}


void test_primitive_data()
{
	Dictionary<int,double> dict(DEFAULT_INT);
	int    key1  = 11;
	int    key2  = 22;
	double data1 = 5.55555678;
	double data2 = 10.11111144;
	double data1_initial = data1;

	dict[key1] = data1;
	dict[key2] = data2;

	data1 = 7.77777777;

	assert(dict[key1] != data1);
	assert(dict[key1] == data1_initial);

	printf("PASSED: test_primitive_data\n");
}


void test_pointer_data()
{
	Dictionary<int,double*> dict(DEFAULT_DOUBLE_POINTER);
	int     key1  = 11;
	int     key2  = 22;
	double  d1    = 5.55555678;
	double  d2    = 10.11111144;
	double *data1 = &d1;
	double *data2 = &d2;

	dict[key1] = data1;
	dict[key2] = data2;

	*data1 = 7.77777777;
	assert(dict[key1] == data1);

	printf("PASSED: test_pointer_data\n");
}


void test_overwrite_data()
{
	Dictionary<int,ulong*> dict(DEFAULT_ULONG_POINTER);
	int    key   = 11;
	ulong  d1    = 60;
	ulong  d2    = 99;
	ulong *data1 = &d1;
	ulong *data2 = &d2;
	ulong *found;

	dict[key] = data1;
	dict[key] = data2;

	assert(dict[key] == data2);
	assert(*(dict[key]) == *data2);

	printf("PASSED: test_overwrite_data\n");
}


void test_pointer_keys()
{
	Dictionary<int*,int> dict(DEFAULT_INT);
	int  k1    = 77;
	int  k2    = 88;
	int *key1  = &k1;
	int *key2  = &k2;
	int  data1 = 10;
	int  data2 = 20;

	dict[key1] = data1;
	dict[key2] = data2;

	assert(dict[key2] == data2);

	key2 = key1;
	assert(dict[key2] == data1);

	printf("PASSED: test_pointer_keys\n");
}


void test_pointer_key_overwrite()
{
	Dictionary<int*,int> turbo(DEFAULT_INT);

	int  same_key = 11;
	int  different_key;

	int *key1 = &same_key;
	int *key2 = &same_key;
	int *key3 = &same_key;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;

	turbo[key1] = data1;

	assert(turbo[key1] == data1);

	same_key = 22;

	assert(turbo[key2] == data1);

	same_key = 33;

	assert(turbo[key3] == data1);

	key1 = &different_key;

	assert(turbo[key1] == DEFAULT_INT);
	assert(turbo[key2] == data1);
	assert(turbo[key3] == data1);

	turbo[key1] = data2;

	assert(turbo[key1] == data2);
	assert(turbo[key2] == data1);
	assert(turbo[key3] == data1);

	key3 = key1;

	assert(turbo[key1] == data2);
	assert(turbo[key2] == data1);
	assert(turbo[key3] == data2);

	printf("PASSED: test_pointer_key_overwrite\n");
}


void test_array_key()
{
	const int KEY_LEN = 7;
	Dictionary<int[KEY_LEN],char> dict(DEFAULT_CHAR);

	int  i;
	char data = 'z';
	int  key1[] = {0,2,2,7,4,5,5};
	int *key2   = (int*) malloc(sizeof(int)*KEY_LEN);

	for (i = 0; i < KEY_LEN; ++i)
		key2[i] = key1[i];

	dict[key1] = data;

	assert(dict[key2] == data);

	/* change last index to test off by one */
	int test_key1[] = {0,2,2,7,3,5,5};
	assert(dict[test_key1] == DEFAULT_CHAR);

	int test_key2[] = {0,2,2,7,4,5,6};
	assert(dict[test_key2] == DEFAULT_CHAR);

	key2[KEY_LEN-1] = 6;
	dict[key2] = data;
	assert(dict[key2] == data);

	assert(dict[test_key1] == DEFAULT_CHAR);
	assert(dict[test_key2] != DEFAULT_CHAR);

	free(key2);
	printf("PASSED: test_array_key\n");
}


void test_multiple_inserts()
{
	const int INSERT_BOUND = 10;
	Dictionary<int,char> dict(DEFAULT_CHAR);

	int  i;
	int  key  = 99;
	char data1 = 'Z';
	char data2 = 'Y';
	char test  = 'F';

	dict[key] = data1;
	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);

	dict[key] = data1;
	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);


	for (i = 0; i < INSERT_BOUND; ++i)
		dict[key] = data1;

	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);

	dict[key] = data2;

	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] != data1);
	assert(dict[key] == data2);

	printf("PASSED: test_multiple_inserts\n");
}


void test_string_key()
{
	const uchar default_uchar = 0xff;

	Dictionary<string,uchar> dict(default_uchar);
	uchar data1 = 0x77;
	uchar data2 = 0x16;
	uchar test  = 0;

	dict["key1"] = data1;
	assert(dict["key1"] != default_uchar);
	assert(dict["key1"] == data1);

	dict["key2"] = data2;
	assert(dict["key2"] != default_uchar);
	assert(dict["key2"] == data2);

	assert(dict["key12"] == default_uchar);
	assert(dict["key22"] == default_uchar);
	assert(dict["key"] == default_uchar);
	assert(dict["ke"] == default_uchar);
	assert(dict["k"] == default_uchar);
	assert(dict[""] == default_uchar);

	printf("PASSED: test_string_key\n");
}


int main()
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

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
	test_array_key();
	test_multiple_inserts();
	test_string_key();

	cout << "\n\nThe program has terminated gracefully\n\n";

	return 0;
}

